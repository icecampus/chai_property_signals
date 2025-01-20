#include <iostream>

#include <chaiscript/chaiscript.hpp>
#include <boost/signals2/signal.hpp>

using namespace boost::signals2;
using namespace chaiscript;


using Signal = boost::signals2::signal<void()>;
using IntSignal = boost::signals2::signal<void(int)>;
using Slot = boost::signals2::slot<void(), boost::function<void()>>;
using Pos = boost::signals2::connect_position;

using CallbackFunction = std::function<void()>;
using IntCallbackFunction = std::function<void(int)>;


class IntEditor
{
public:
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


private:
    int i = 0;
};


class Label
{
public:
    void set_text(int i)
    {
        std::cout << "update text";
    }

    int get_text() { return 0; }
    IntSignal on_text_updated;
};

void connect(Signal* signal, CallbackFunction callback)
{
    signal->connect(callback);
}

void connect_int(IntSignal& signal, IntCallbackFunction& callback)
{
    signal.connect(callback);
}

int main()
{
    ChaiScript chai;

    chai.add(chaiscript::user_type<IntEditor>(), "IntEditor");
    chai.add(constructor<IntEditor()>(), "IntEditor");
    chai.add(chaiscript::fun(&IntEditor::set_i), "set_i");
    chai.add(chaiscript::fun(&IntEditor::get_i), "get_i");
    chai.add(chaiscript::fun(&IntEditor::get_on_i_updated), "get_on_i_updated");

    chai.add(chaiscript::user_type<Label>(), "Label");
    chai.add(constructor<Label()>(), "Label");
    chai.add(chaiscript::fun(&Label::set_text), "set_text");


    chai.add(chaiscript::fun([](IntSignal& editor, const IntCallbackFunction& callback) {
        editor.connect(callback);
        }), "connect");

    chai.add(chaiscript::fun([](Signal& editor, const CallbackFunction& callback) {
        editor.connect(callback);
        }), "connect");

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

