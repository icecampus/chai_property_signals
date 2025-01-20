#include <iostream>

#include <chaiscript/chaiscript.hpp>
#include <boost/signals2/signal.hpp>

using namespace boost::signals2;
using namespace chaiscript;


class LineEditor
{
public:
    using StringSignal = boost::signals2::signal<void(std::string)>;

    LineEditor(){}
    void set_text(std::string i_)
    {
        i = i_;
        on_text_updated(i);
    }

    std::string get_text() { return i; }
    StringSignal on_text_updated;

    StringSignal& get_on_text_updated() {
        return on_text_updated;
    }

    static void registerClass(ChaiScript& chai)
    {
        chai.add(chaiscript::user_type<LineEditor>(), "LineEditor");
        chai.add(constructor<LineEditor()>(), "LineEditor");
        chai.add(chaiscript::fun(&LineEditor::set_text), "set_text");
        chai.add(chaiscript::fun(&LineEditor::get_text), "get_text");
        chai.add(chaiscript::fun(&LineEditor::get_on_text_updated), "get_on_text_updated");
    }

private:
    std::string i;
};


class Label
{
public:
    using StringSignal = boost::signals2::signal<void(std::string)>;
    void set_text(std::string text_)
    {
        if (text!=text_)
        {
            text = text_;
            on_text_updated(text);
        }
    }

    std::string get_text() { return text; }
    StringSignal on_text_updated;

    static void registerClass(ChaiScript& chai)
    {
        chai.add(chaiscript::user_type<Label>(), "Label");
        chai.add(constructor<Label()>(), "Label");
        chai.add(chaiscript::fun(&Label::set_text), "set_text");
        chai.add(chaiscript::fun(&Label::get_text), "get_text");
    }
private:
    std::string text;
};

template<typename T>
void register_type_for_connection(ChaiScript& chai)
{
    chai.add(chaiscript::fun([](boost::signals2::signal<void(T)>& signal, const std::function<void(T)>& callback) {
        signal.connect(callback);
        }), "connect");
}


int main()
{
    ChaiScript chai;

    register_type_for_connection<int>(chai);
    register_type_for_connection<std::string>(chai);
    LineEditor::registerClass(chai);
    Label::registerClass(chai);

    try 
    {

        const char* script = R"(
    
            // LineEditor{ id: lineEditor }
            var editor = LineEditor();
           
            // Label{ id: label; text:lineEditor.text }
            var label = Label();

            connect(editor.get_on_text_updated(), fun[label](value)
            { 
                label.set_text(value);
            });

            //use            
            editor.set_text("42");

            print(label.get_text());

        )";
        chai.eval(script);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

