#include <iostream>

#include <chaiscript/chaiscript.hpp>
#include <boost/signals2/signal.hpp>

using namespace boost::signals2;
using namespace chaiscript;

// �������, ������� ����� ������������ � �������
void on_event()
{
    std::cout << "Event come!" << std::endl;
}


using Signal = boost::signals2::signal<void()>;
using Slot = boost::signals2::slot<void(), boost::function<void()>>;
using Pos = boost::signals2::connect_position;
using CallbackFunction = std::function<void()>;

void connect(Signal* signal, CallbackFunction callback)
{
    signal->connect(callback);
}

int main()
{
    // �������� ChaiScript ������
    ChaiScript chai;



    // ������� ������� � ������� � ChaiScript
    chai.add(chaiscript::fun(&on_event), "on_event");


    chai.add(chaiscript::fun(&connect), "connect");


    // ����������� ���� Signal � ChaiScript
    chai.add(user_type<Signal>(), "Signal");

    chai.add(constructor<Signal()>(), "Signal");

    //���������� ������ connect ��� �������
    chai.add(
        fun(static_cast<connection(Signal::*)(const Slot&, Pos)>(&Signal::connect)),
        "connect"
    );

    chai.add(fun(static_cast<void(signal<void()>::*) ()>(&signal<void()>::operator())), "emit");

    

    try {

        const char* script = R"(

            var sig = Signal();

            connect(sig, fun()
            { 
                print(1) 
            });

            sig.emit();

        )";
        chai.eval(script);



    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    //// ����������� ����� � �������

    //// ������������ �������
    //chai.eval(R"(
    //    sig.emit();
    //)");


}