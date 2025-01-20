#include <iostream>

#include <chaiscript/chaiscript.hpp>
#include <boost/signals2/signal.hpp>

using namespace boost::signals2;
using namespace chaiscript;


class IntEditor
{
public:
    using IntSignal = boost::signals2::signal<void(int)>;

    IntEditor(){}
    void set_i(int i_)
    {
        i = i_;
        on_i_updated(i);
    }

    int get_i() { return i; }
    IntSignal on_i_updated;

    IntSignal& get_on_i_updated() {
        return on_i_updated;
    }


    static void registerClass(ChaiScript& chai)
    {
        chai.add(chaiscript::user_type<IntEditor>(), "IntEditor");
        chai.add(constructor<IntEditor()>(), "IntEditor");
        chai.add(chaiscript::fun(&IntEditor::set_i), "set_i");
        chai.add(chaiscript::fun(&IntEditor::get_i), "get_i");
        chai.add(chaiscript::fun(&IntEditor::get_on_i_updated), "get_on_i_updated");
    }

private:
    int i = 0;
};


class Label
{
public:
    using IntSignal = boost::signals2::signal<void(int)>;
    void set_text(int i)
    {
        std::cout << "update text";
    }

    int get_text() { return 0; }
    IntSignal on_text_updated;

    static void registerClass(ChaiScript& chai)
    {
        chai.add(chaiscript::user_type<Label>(), "Label");
        chai.add(constructor<Label()>(), "Label");
        chai.add(chaiscript::fun(&Label::set_text), "set_text");
    }
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
    IntEditor::registerClass(chai);
    Label::registerClass(chai);

    try {

        const char* script = R"(

           var editor = IntEditor();
           var label = Label();

           connect(editor.get_on_i_updated(), fun[label](value)
           { 
               label.set_text(value);
           });

            editor.set_i(42);

        )";
        chai.eval(script);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

