#ifndef GTK_APP_MANAGER_H
#define GTK_APP_MANAGER_H

#include <gtk/gtk.h>

class GTKAppManager
{
	private:
	GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *menu_bar;
    GtkToolItem *load_key_set;
    GtkToolItem *primary_key;
    GtkToolItem *secondary_key;
    GtkToolItem *save_key_set;
	GtkToolItem *separator;
	GtkToolItem *load_input;
    GtkToolItem *cypher;
    GtkToolItem *decypher;
    GtkToolItem *save_output;
	GtkToolItem *help;
    GtkToolItem *separator2;
    GtkToolItem *quit;
    GtkWidget *set;
    GtkWidget *input_frame;
    GtkWidget *input_hbox;
    GtkWidget *input_vbox;
    GtkWidget *input;
    GtkWidget *output_frame;
    GtkWidget *output_hbox;
    GtkWidget *output_vbox;
    GtkWidget *output;
    GtkWidget *status_bar;
    public:
    GTKAppManager(unsigned int height = 1024, unsigned int width = 960);
    private:
    void mainWindow(unsigned int height = 1024, unsigned int width = 960);
    
    void menuBar();
    
    void cypherMenu();
    void IOMenu();
    void extraMenu();
    
    void IO();
    
    void statusBar();
    
    void actionLinking();
    public:
    ~GTKAppManager();
};

#endif
