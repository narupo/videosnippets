#include <gtkmm.h>

class App : public Gtk::Window {
private:
    Gtk::Label mLabel;

public:
    App();
};

App::App()
    : Gtk::Window()
    , mLabel("Hello, World!") {
    set_title("Hello Window");
    mLabel.set_margin_start(50);
    mLabel.set_margin_end(50);
    mLabel.set_margin_top(50);
    mLabel.set_margin_bottom(50);
    add(mLabel);
    show_all_children();
}

int main(int argc, char *argv[]) {
    Gtk::Main kit(argc, argv);
    App app;
    Gtk::Main::run(app);
}
