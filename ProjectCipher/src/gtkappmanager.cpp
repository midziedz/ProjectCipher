#include "../include/gtkappmanager.h"

extern void load_key_set_from_file(GtkWidget *widget, gpointer window);
extern void set_primary_key(GtkWidget *widget, gpointer window);
extern void set_secondary_key(GtkWidget *widget, gpointer window);
extern void save_key_set_to_file(GtkWidget *widget, gpointer window);
extern void load_input_from_file(GtkWidget *widget, gpointer *inputArgs);
extern void do_cypher(GtkWidget *widget, gpointer *args);
extern void do_decypher(GtkWidget *widget, gpointer *args);
extern void save_output_to_file(GtkWidget *widget, gpointer *outputArgs);



void GTKAppManager::mainWindow(unsigned int height, unsigned int width)
{
	this->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(this->window), "Vigenère Cypher Centre");
    gtk_window_set_default_size (GTK_WINDOW(this->window), height, width);

    this->vbox = gtk_vbox_new(FALSE, 2);
    gtk_container_add(GTK_CONTAINER(this->window), this->vbox);
}

void GTKAppManager::menuBar()
{
	this->menu_bar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(this->menu_bar), GTK_TOOLBAR_ICONS);

    gtk_container_set_border_width(GTK_CONTAINER(this->menu_bar), 2);
    
    this->cypherMenu();
    
    this->separator = gtk_separator_tool_item_new();
    gtk_toolbar_insert(GTK_TOOLBAR(this->menu_bar), this->separator, -1);
    
    this->IOMenu();
    
    this->separator2 = gtk_separator_tool_item_new();
    gtk_toolbar_insert(GTK_TOOLBAR(this->menu_bar), this->separator2, -1);
    
    this->extraMenu();
}

void GTKAppManager::cypherMenu()
{
	this->load_key_set = gtk_tool_button_new_from_stock(GTK_STOCK_FIND);
    gtk_widget_set_has_tooltip(GTK_WIDGET(this->load_key_set), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(this->load_key_set), "Load key set from file");
    gtk_toolbar_insert(GTK_TOOLBAR(this->menu_bar), this->load_key_set, -1);

    this->primary_key = gtk_tool_button_new_from_stock(GTK_STOCK_DIALOG_AUTHENTICATION);
    gtk_widget_set_has_tooltip(GTK_WIDGET(this->primary_key), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(this->primary_key), "Set primary key");
    gtk_toolbar_insert(GTK_TOOLBAR(this->menu_bar), this->primary_key, -1);

    this->secondary_key = gtk_tool_button_new_from_stock(GTK_STOCK_JUSTIFY_FILL);
    gtk_widget_set_has_tooltip(GTK_WIDGET(this->secondary_key), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(this->secondary_key), "Set secondary key");
    gtk_toolbar_insert(GTK_TOOLBAR(this->menu_bar), this->secondary_key, -1);

    this->save_key_set = gtk_tool_button_new_from_stock(GTK_STOCK_EDIT);
    gtk_widget_set_has_tooltip(GTK_WIDGET(this->save_key_set), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(this->save_key_set), "Save key set to file");
    gtk_toolbar_insert(GTK_TOOLBAR(this->menu_bar), this->save_key_set, -1);
}

void GTKAppManager::IOMenu()
{
	this->load_input = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
    gtk_widget_set_has_tooltip(GTK_WIDGET(this->load_input), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(this->load_input), "Load input");
    gtk_toolbar_insert(GTK_TOOLBAR(this->menu_bar), this->load_input, -1);

    this->cypher = gtk_tool_button_new_from_stock(GTK_STOCK_EXECUTE);
    gtk_widget_set_has_tooltip(GTK_WIDGET(this->cypher), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(this->cypher), "Cypher input");
    gtk_toolbar_insert(GTK_TOOLBAR(this->menu_bar), this->cypher, -1);

    this->decypher = gtk_tool_button_new_from_stock(GTK_STOCK_REFRESH);
    gtk_widget_set_has_tooltip(GTK_WIDGET(this->decypher), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(this->decypher), "Decypher input");
    gtk_toolbar_insert(GTK_TOOLBAR(this->menu_bar), this->decypher, -1);

    this->save_output = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
    gtk_widget_set_has_tooltip(GTK_WIDGET(this->save_output), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(this->save_output), "Save output");
    gtk_toolbar_insert(GTK_TOOLBAR(this->menu_bar), this->save_output, -1);
}

void GTKAppManager::extraMenu()
{
	this->help = gtk_tool_button_new_from_stock(GTK_STOCK_HELP);
    gtk_widget_set_has_tooltip(GTK_WIDGET(this->help), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(this->help), "Help");
    gtk_toolbar_insert(GTK_TOOLBAR(this->menu_bar), this->help, -1);

    this->quit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
    gtk_widget_set_has_tooltip(GTK_WIDGET(this->quit), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(this->quit), "Quit");
    gtk_toolbar_insert(GTK_TOOLBAR(this->menu_bar), this->quit, -1);

    gtk_box_pack_start(GTK_BOX(this->vbox), this->menu_bar, FALSE, FALSE, 2);
}

void GTKAppManager::IO()
{
	this->set = gtk_hbox_new(TRUE, 2);
	gtk_box_pack_start(GTK_BOX(this->vbox), this->set, TRUE, TRUE, 5);

	this->input_frame = gtk_frame_new("Place Your input here:");
	gtk_frame_set_shadow_type(GTK_FRAME(this->input_frame), GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(this->set), this->input_frame, TRUE, TRUE, 5);
	this->input_hbox = gtk_hbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(this->input_frame), this->input_hbox);
	this->input_vbox = gtk_vbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(this->input_hbox), this->input_vbox, TRUE, TRUE, 5);

    this->input = gtk_text_view_new();

    gtk_text_view_set_editable(GTK_TEXT_VIEW(this->input), TRUE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(this->input), TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(this->input), GTK_WRAP_WORD);
    //input_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(input));

    gtk_box_pack_start(GTK_BOX(this->input_vbox), this->input, TRUE, TRUE, 5);

    this->output_frame = gtk_frame_new("Here is Your output:");
    gtk_frame_set_shadow_type(GTK_FRAME(this->output_frame), GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(set), this->output_frame, TRUE, TRUE, 5);
	this->output_hbox = gtk_hbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(this->output_frame), this->output_hbox);
	this->output_vbox = gtk_vbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(this->output_hbox), this->output_vbox, TRUE, TRUE, 5);

    this->output = gtk_text_view_new();

    gtk_text_view_set_editable(GTK_TEXT_VIEW(this->output), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(this->output), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(this->output), GTK_WRAP_WORD);

    gtk_box_pack_start(GTK_BOX(this->output_vbox), this->output, TRUE, TRUE, 5);
}

void GTKAppManager::statusBar()
{
	this->status_bar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(this->vbox), this->status_bar, FALSE, FALSE, 2);
	gchar* copyright = (gchar*)("Copyright © 2020 Michał Dziedzic");
	
	gtk_statusbar_push(GTK_STATUSBAR(this->status_bar), gtk_statusbar_get_context_id(GTK_STATUSBAR(this->status_bar), copyright), copyright);
}

void GTKAppManager::actionLinking()
{
	g_signal_connect(G_OBJECT(this->load_key_set), "clicked", G_CALLBACK(load_key_set_from_file), window);
    g_signal_connect(G_OBJECT(this->primary_key), "clicked", G_CALLBACK(set_primary_key), window);
    g_signal_connect(G_OBJECT(this->secondary_key), "clicked", G_CALLBACK(set_secondary_key), window);
    g_signal_connect(G_OBJECT(this->save_key_set), "clicked", G_CALLBACK(save_key_set_to_file), window);

    gpointer *inputArgs = new gpointer[2];
    inputArgs[0] = this->window;
    inputArgs[1] = this->input;
    g_signal_connect(G_OBJECT(this->load_input), "clicked", G_CALLBACK(load_input_from_file), inputArgs);

    gpointer *cypherArgs = new gpointer[3];
    cypherArgs[0] = this->window;
    cypherArgs[1] = this->input;
    cypherArgs[2] = this->output;
    g_signal_connect(G_OBJECT(this->cypher), "clicked", G_CALLBACK(do_cypher), cypherArgs);
    g_signal_connect(G_OBJECT(this->decypher), "clicked", G_CALLBACK(do_decypher), cypherArgs);

    gpointer *outputArgs = new gpointer[2];
    outputArgs[0] = this->window;
    outputArgs[1] = this->output;
    g_signal_connect(G_OBJECT(this->save_output), "clicked", G_CALLBACK(save_output_to_file), outputArgs);

    g_signal_connect(G_OBJECT(this->quit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(this->window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

GTKAppManager::GTKAppManager(unsigned int height, unsigned int width)
{
    this->mainWindow(height, width);
	
    this->menuBar();
	
    this->IO();
    
    this->statusBar();

    this->actionLinking();

    gtk_widget_show_all(this->window);
}
GTKAppManager::~GTKAppManager()
{
	
}
