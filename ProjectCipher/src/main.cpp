#include <gtk/gtk.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
//#include <locale>
//#include <codecvt>

#include "../include/cypher.h"
#include "../include/cyphermanager.h"
#include "../include/iomanager.h"
#include "../include/gtkappmanager.h"

using namespace std;

/*wstring widenUp(string narrowString)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	wstring wide = converter.from_bytes(narrowString);
	return wide;
}
string narrowDown(wstring wideString)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	string narrow = converter.to_bytes(wideString);
	return narrow;
}*/
// All support function definitions:
bool checkIfLegal(string alphabet, int stop_index, int search_index)
{
    char sign = alphabet[search_index];
    for(int i = 0; i <= stop_index; i++)
    {
        if(sign == alphabet[i]) return false;
    }
    return true;
}
string shuffleTheAlphabet(string alphabet)
{
    srand(time(NULL));
    string result = alphabet;
    int new_pos;
    for(unsigned int i = 0; i < alphabet.length(); i++)
    {
        new_pos = rand()%alphabet.length();
        if(checkIfLegal(result, i - 1, new_pos))
        {
            result[new_pos] = alphabet[i];
        }
    }
    return result;
}


// All gtk-related function declarations:
void load_key_set_from_file(GtkWidget *widget, gpointer window);
void set_primary_key(GtkWidget *widget, gpointer window);
void set_secondary_key(GtkWidget *widget, gpointer window);
void save_key_set_to_file(GtkWidget *widget, gpointer window);
void load_input_from_file(GtkWidget *widget, gpointer *inputArgs);
void do_cypher(GtkWidget *widget, gpointer *args);
void do_decypher(GtkWidget *widget, gpointer *args);
void save_output_to_file(GtkWidget *widget, gpointer *outputArgs);

// Global variables (to improve):
VigenereCypher justCypher;
CypherManager justALoader;
IOManager simpleManager;

string firstVerse = "";
string VigenereKeyFileName = "";
string base = "";
int main(int argc, char** argv)
{
	char real[50];
	realpath(argv[0], real);
	string argv_str(real);
    base = argv_str.substr(0, argv_str.find_last_of("/") + 1);
    cout << base << endl;
    justALoader.setFile(base + "assets/keySets/default.ks", CypherManager::FileType::KeySet);
    justALoader.flushToCypher(justCypher);

    gtk_init(&argc, &argv);
    
    GTKAppManager gtkAppManager;
	
    gtk_main();

    return 0;
}

// Function definitions:
// Loading key set from file
void key_set_loader(GtkWidget *widget, gpointer *args)
{
	GtkWidget *dialog;
    string KeySetFileName = "";
    const gchar* key_set_text = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(args[1]));
    if(key_set_text == NULL)
    {
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "No key set file chosen!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_widget_destroy(GTK_WIDGET(args[0]));
        return;
    }
    KeySetFileName = key_set_text;
    //cout << VigenereKeyFileName << endl;

    KeySetFileName = KeySetFileName.substr(7, VigenereKeyFileName.length() - 1);
    justALoader.setFile(KeySetFileName, CypherManager::FileType::KeySet);
    justALoader.flushToCypher(justCypher);
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Key set was loaded successfully!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(GTK_WIDGET(args[0]));
}
void load_key_set_from_file(GtkWidget *widget, gpointer window)
{
	GtkWidget *new_window;
    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *key_set_frame;
    GtkWidget *inner_vbox;
    GtkWidget *key_set_file_chooser;
    GtkWidget *submit_button;
    GtkWidget *sup_image;

    GtkSettings *default_settings = gtk_settings_get_default();
    g_object_set(default_settings, "gtk-button-images", TRUE, NULL);

    new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(new_window), "Key settings");
    gtk_window_set_default_size(GTK_WINDOW(new_window), 250, 50);
    hbox = gtk_hbox_new(TRUE, 5);
    gtk_container_add(GTK_CONTAINER(new_window), hbox);
    vbox = gtk_vbox_new(TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 5);

    key_set_frame = gtk_frame_new("Choose Your key set file:");
	gtk_frame_set_shadow_type(GTK_FRAME(key_set_frame), GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(vbox), key_set_frame, TRUE, TRUE, 5);

	inner_vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(key_set_frame), inner_vbox);

    key_set_file_chooser = gtk_file_chooser_button_new("Key set file", GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_file_chooser_set_current_folder_uri(GTK_FILE_CHOOSER(key_set_file_chooser), (string("file://") + base + "assets/keySets").c_str());
    gtk_box_pack_start(GTK_BOX(inner_vbox), key_set_file_chooser, TRUE, TRUE, 5);

    submit_button = gtk_button_new_with_label("Apply");
    gtk_box_pack_start(GTK_BOX(inner_vbox), submit_button, TRUE, TRUE, 5);

    sup_image = gtk_image_new_from_stock(GTK_STOCK_APPLY, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(submit_button), GTK_WIDGET(sup_image));
    gtk_button_set_image_position(GTK_BUTTON(submit_button), GTK_POS_LEFT);

    gpointer *args = new gpointer[2];
    args[0] = new_window;
    args[1] = key_set_file_chooser;
    g_signal_connect(G_OBJECT(submit_button), "clicked", G_CALLBACK(key_set_loader), args);

    gtk_widget_show_all(new_window);
}
// Setting custom primary key
void primary_key_setter(GtkWidget *widget, gpointer *args)
{
    GtkWidget *dialog;
    string firstVerse = "";
    const gchar* primary_key_text = gtk_entry_get_text(GTK_ENTRY(args[1]));
    firstVerse = primary_key_text;
    if(firstVerse == "")
    {
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Sign set cannot be empty!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_widget_destroy(GTK_WIDGET(args[0]));
        return;
    }
    firstVerse = firstVerse + "\n";
    justALoader.setFile(firstVerse, CypherManager::FileType::JustSignSet);
    justALoader.flushToCypher(justCypher);
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Sign set was set successfully!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(GTK_WIDGET(args[0]));
}
void primary_key_importer(GtkWidget *widget, gpointer *args)
{
    GtkWidget *dialog;
    string importFile = "";
    string firstVerse = "";
    const gchar* primary_key_text = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(args[1]));
    importFile = primary_key_text;
    importFile = importFile.substr(7, importFile.length() - 1);
    if(importFile == "")
    {
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "No import file chosen!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_widget_destroy(GTK_WIDGET(args[0]));
        return;
    }
    justALoader.setFile(importFile, CypherManager::FileType::SignSet);
    justALoader.flushToCypher(justCypher);
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Sign set was set successfully!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(GTK_WIDGET(args[0]));
}
void set_primary_key(GtkWidget *widget, gpointer window)
{
    GtkWidget *new_window;
    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *alphabet_frame;
    GtkWidget *inner_vbox;
    GtkWidget *alphabet_entry;
    GtkWidget *submit_button;
    GtkWidget *import_frame;
    GtkWidget *inner_vbox2;
    GtkWidget *sign_set_file_chooser;
    GtkWidget *submit_button2;

    GtkWidget *sup_image;
    GtkWidget *sup_image2;

    GtkSettings *default_settings = gtk_settings_get_default();
    g_object_set(default_settings, "gtk-button-images", TRUE, NULL);

    new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(new_window), "Sign set settings");
    gtk_window_set_default_size(GTK_WINDOW(new_window), 250, 50);
    hbox = gtk_hbox_new(TRUE, 5);
    gtk_container_add(GTK_CONTAINER(new_window), hbox);
    vbox = gtk_vbox_new(TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 5);

    alphabet_frame = gtk_frame_new("Input Your sign set:");
	gtk_frame_set_shadow_type(GTK_FRAME(alphabet_frame), GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(vbox), alphabet_frame, TRUE, TRUE, 5);

	inner_vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(alphabet_frame), inner_vbox);

    alphabet_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(inner_vbox), alphabet_entry, TRUE, TRUE, 5);

    submit_button = gtk_button_new_with_label("Apply");
    gtk_box_pack_start(GTK_BOX(inner_vbox), submit_button, FALSE, FALSE, 5);

    sup_image = gtk_image_new_from_stock(GTK_STOCK_APPLY, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(submit_button), GTK_WIDGET(sup_image));
    gtk_button_set_image_position(GTK_BUTTON(submit_button), GTK_POS_LEFT);

	import_frame = gtk_frame_new("Or choose from predefined sign sets:");
	gtk_frame_set_shadow_type(GTK_FRAME(import_frame), GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(vbox), import_frame, TRUE, TRUE, 5);

	inner_vbox2 = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(import_frame), inner_vbox2);

    sign_set_file_chooser = gtk_file_chooser_button_new("Key file", GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_file_chooser_set_current_folder_uri(GTK_FILE_CHOOSER(sign_set_file_chooser), (string("file://") + base + "assets/signSets").c_str());
    gtk_box_pack_start(GTK_BOX(inner_vbox2), sign_set_file_chooser, TRUE, TRUE, 5);

    submit_button2 = gtk_button_new_with_label("Load");
    gtk_box_pack_start(GTK_BOX(inner_vbox2), submit_button2, TRUE, TRUE, 5);

    sup_image2 = gtk_image_new_from_stock(GTK_STOCK_APPLY, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(submit_button2), GTK_WIDGET(sup_image2));
    gtk_button_set_image_position(GTK_BUTTON(submit_button2), GTK_POS_LEFT);

    gpointer *inputArgs = new gpointer[2];
    inputArgs[0] = new_window;
    inputArgs[1] = alphabet_entry;
    g_signal_connect(G_OBJECT(submit_button), "clicked", G_CALLBACK(primary_key_setter), inputArgs);

    gpointer *importArgs = new gpointer[2];
    importArgs[0] = new_window;
    importArgs[1] = sign_set_file_chooser;
    g_signal_connect(G_OBJECT(submit_button2), "clicked", G_CALLBACK(primary_key_importer), importArgs);

    gtk_widget_show_all(new_window);
}
// Setting custom secondary key
void secondary_key_setter(GtkWidget *widget, gpointer *args)
{
    GtkWidget *dialog;
    string VigenereKeyFileName = "";
    const gchar* secondary_key_text = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(args[1]));
    if(secondary_key_text == NULL)
    {
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "No key file chosen!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_widget_destroy(GTK_WIDGET(args[0]));
        return;
    }
    VigenereKeyFileName = secondary_key_text;
    //cout << VigenereKeyFileName << endl;
    justALoader.setFile(VigenereKeyFileName, CypherManager::FileType::JustVigenereFile);
    justALoader.flushToCypher(justCypher);
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Vigenere key file was set successfully!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(GTK_WIDGET(args[0]));
}
void set_secondary_key(GtkWidget *widget, gpointer window)
{
    GtkWidget *new_window;
    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *key_frame;
    GtkWidget *inner_vbox;
    GtkWidget *key_file_chooser;
    GtkWidget *submit_button;
    GtkWidget *sup_image;

    GtkSettings *default_settings = gtk_settings_get_default();
    g_object_set(default_settings, "gtk-button-images", TRUE, NULL);

    new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(new_window), "Key settings");
    gtk_window_set_default_size(GTK_WINDOW(new_window), 250, 50);
    hbox = gtk_hbox_new(TRUE, 5);
    gtk_container_add(GTK_CONTAINER(new_window), hbox);
    vbox = gtk_vbox_new(TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 5);

    key_frame = gtk_frame_new("Choose Your key file:");
	gtk_frame_set_shadow_type(GTK_FRAME(key_frame), GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(vbox), key_frame, TRUE, TRUE, 5);

	inner_vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(key_frame), inner_vbox);

    key_file_chooser = gtk_file_chooser_button_new("Key file", GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_file_chooser_set_current_folder_uri(GTK_FILE_CHOOSER(key_file_chooser), (string("file://") + base + "assets/vigenereKeys").c_str());
    gtk_box_pack_start(GTK_BOX(inner_vbox), key_file_chooser, TRUE, TRUE, 5);

    submit_button = gtk_button_new_with_label("Apply");
    gtk_box_pack_start(GTK_BOX(inner_vbox), submit_button, TRUE, TRUE, 5);

    sup_image = gtk_image_new_from_stock(GTK_STOCK_APPLY, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(submit_button), GTK_WIDGET(sup_image));
    gtk_button_set_image_position(GTK_BUTTON(submit_button), GTK_POS_LEFT);

    gpointer *args = new gpointer[2];
    args[0] = new_window;
    args[1] = key_file_chooser;
    g_signal_connect(G_OBJECT(submit_button), "clicked", G_CALLBACK(secondary_key_setter), args);

    gtk_widget_show_all(new_window);
}
// Saving key set to file
void key_set_saver(GtkWidget *widget, gpointer *args)
{
	GtkWidget *dialog;
    string KeySetFileName = "";
    const gchar* key_set_text = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(args[1]));
    if(key_set_text == NULL)
    {
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "No key set file chosen!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_widget_destroy(GTK_WIDGET(args[0]));
        return;
    }
    KeySetFileName = key_set_text;
    //cout << VigenereKeyFileName << endl;

    KeySetFileName = KeySetFileName.substr(7, KeySetFileName.length() - 1);
    justALoader.setFile(KeySetFileName, CypherManager::FileType::KeySet);
    justALoader.saveFromCypher(justCypher);
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Key set was saved successfully!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(GTK_WIDGET(args[0]));
}
void save_key_set_to_file(GtkWidget *widget, gpointer window)
{
	GtkWidget *new_window;
    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *key_set_frame;
    GtkWidget *inner_vbox;
    GtkWidget *key_set_file_chooser;
    GtkWidget *submit_button;
    GtkWidget *sup_image;

    GtkSettings *default_settings = gtk_settings_get_default();
    g_object_set(default_settings, "gtk-button-images", TRUE, NULL);

    new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(new_window), "Key settings");
    gtk_window_set_default_size(GTK_WINDOW(new_window), 720, 360);
    hbox = gtk_hbox_new(TRUE, 5);
    gtk_container_add(GTK_CONTAINER(new_window), hbox);
    vbox = gtk_vbox_new(TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 5);

    key_set_frame = gtk_frame_new("Choose Your key set file:");
	gtk_frame_set_shadow_type(GTK_FRAME(key_set_frame), GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(vbox), key_set_frame, TRUE, TRUE, 5);

	inner_vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(key_set_frame), inner_vbox);

    key_set_file_chooser = gtk_file_chooser_widget_new(GTK_FILE_CHOOSER_ACTION_SAVE);
    gtk_file_chooser_set_current_folder_uri(GTK_FILE_CHOOSER(key_set_file_chooser), (string("file://") + base + "assets/keySets").c_str());
    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(key_set_file_chooser), TRUE);
    gtk_box_pack_start(GTK_BOX(inner_vbox), key_set_file_chooser, TRUE, TRUE, 5);

    submit_button = gtk_button_new_with_label("Apply");
    gtk_box_pack_start(GTK_BOX(inner_vbox), submit_button, FALSE, FALSE, 5);

    sup_image = gtk_image_new_from_stock(GTK_STOCK_APPLY, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(submit_button), GTK_WIDGET(sup_image));
    gtk_button_set_image_position(GTK_BUTTON(submit_button), GTK_POS_LEFT);

    gpointer *args = new gpointer[2];
    args[0] = new_window;
    args[1] = key_set_file_chooser;
    g_signal_connect(G_OBJECT(submit_button), "clicked", G_CALLBACK(key_set_saver), args);

    gtk_widget_show_all(new_window);
}
// Loading input from file
void input_loader(GtkWidget *widget, gpointer *args)
{
	GtkWidget *dialog;
    string InputFileName = "";
    const gchar* input_text = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(args[2]));
    if(input_text == NULL)
    {
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "No key set file chosen!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_widget_destroy(GTK_WIDGET(args[0]));
        return;
    }
    InputFileName = input_text;
    //cout << VigenereKeyFileName << endl;
    string input = "";
    simpleManager.setFile(InputFileName, IOManager::IOAction::Load);
    simpleManager.loadToString(input);
    simpleManager.setFile("", IOManager::IOAction::Inactive);
	GtkTextBuffer *input_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(args[1]));
	gtk_text_buffer_set_text(GTK_TEXT_BUFFER(input_buffer), input.c_str(), -1);
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Input was loaded successfully!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(GTK_WIDGET(args[0]));
}
void load_input_from_file(GtkWidget *widget, gpointer *inputArgs)
{
	GtkWidget *new_window;
    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *input_frame;
    GtkWidget *inner_vbox;
    GtkWidget *input_file_chooser;
    GtkWidget *submit_button;
    GtkWidget *sup_image;

    GtkSettings *default_settings = gtk_settings_get_default();
    g_object_set(default_settings, "gtk-button-images", TRUE, NULL);

    new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(new_window), "Input loading");
    gtk_window_set_default_size(GTK_WINDOW(new_window), 250, 50);
    hbox = gtk_hbox_new(TRUE, 5);
    gtk_container_add(GTK_CONTAINER(new_window), hbox);
    vbox = gtk_vbox_new(TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 5);

    input_frame = gtk_frame_new("Choose Your input file:");
	gtk_frame_set_shadow_type(GTK_FRAME(input_frame), GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(vbox), input_frame, TRUE, TRUE, 5);

	inner_vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(input_frame), inner_vbox);

    input_file_chooser = gtk_file_chooser_button_new("Key set file", GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_box_pack_start(GTK_BOX(inner_vbox), input_file_chooser, TRUE, TRUE, 5);

    submit_button = gtk_button_new_with_label("Apply");
    gtk_box_pack_start(GTK_BOX(inner_vbox), submit_button, TRUE, TRUE, 5);

    sup_image = gtk_image_new_from_stock(GTK_STOCK_APPLY, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(submit_button), GTK_WIDGET(sup_image));
    gtk_button_set_image_position(GTK_BUTTON(submit_button), GTK_POS_LEFT);

    gpointer *args = new gpointer[3];
    args[0] = new_window;
    args[1] = inputArgs[1];
    args[2] = input_file_chooser;
    g_signal_connect(G_OBJECT(submit_button), "clicked", G_CALLBACK(input_loader), args);

    gtk_widget_show_all(new_window);
}
// Actual cypher
void do_cypher(GtkWidget *widget, gpointer *args)
{
    GtkTextBuffer *input_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(args[1]));
    GtkTextIter start_iter;
    GtkTextIter end_iter;
    gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(input_buffer), &start_iter);
    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(input_buffer), &end_iter);
    const gchar* input_text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(input_buffer), &start_iter, &end_iter, TRUE);
    //cout << input_text << endl;
    string input = input_text;
    //cout << input << endl;
    string output = justCypher.cypher(input);
	GtkTextBuffer *output_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(args[2]));
	gtk_text_buffer_set_text(GTK_TEXT_BUFFER(output_buffer), output.c_str(), -1);
}
// Actual decypher
void do_decypher(GtkWidget *widget, gpointer *args)
{
    GtkTextBuffer *input_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(args[1]));
    GtkTextIter start_iter;
    GtkTextIter end_iter;
    gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(input_buffer), &start_iter);
    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(input_buffer), &end_iter);
    const gchar* input_text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(input_buffer), &start_iter, &end_iter, TRUE);
    //cout << input_text << endl;
    string input = input_text;
    //cout << input << endl;
    string output = justCypher.decypher(input);
    GtkTextBuffer *output_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(args[2]));
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(output_buffer), output.c_str(), -1);
}
// Saving output to file
void output_saver(GtkWidget *widget, gpointer *args)
{
	GtkWidget *dialog;
    string OutputFileName = "";
    const gchar* output_text = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(args[2]));
    if(output_text == NULL)
    {
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "No key set file chosen!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_widget_destroy(GTK_WIDGET(args[0]));
        return;
    }
    OutputFileName = output_text;
    //cout << VigenereKeyFileName << endl;
    GtkTextBuffer *output_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(args[1]));
    GtkTextIter start_iter;
    GtkTextIter end_iter;
    gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(output_buffer), &start_iter);
    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(output_buffer), &end_iter);
    const gchar* output_text2 = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(output_buffer), &start_iter, &end_iter, TRUE);
    string output = output_text2;
    simpleManager.setFile(OutputFileName, IOManager::IOAction::Save);
    simpleManager.saveFromString(output);
    simpleManager.setFile("", IOManager::IOAction::Inactive);
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Output was saved successfully!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(GTK_WIDGET(args[0]));
}
void save_output_to_file(GtkWidget *widget, gpointer *outputArgs)
{
	GtkWidget *new_window;
    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *output_frame;
    GtkWidget *inner_vbox;
    GtkWidget *output_file_chooser;
    GtkWidget *submit_button;
    GtkWidget *sup_image;

    GtkSettings *default_settings = gtk_settings_get_default();
    g_object_set(default_settings, "gtk-button-images", TRUE, NULL);

    new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(new_window), "Output saving");
    gtk_window_set_default_size(GTK_WINDOW(new_window), 720, 360);
    hbox = gtk_hbox_new(TRUE, 5);
    gtk_container_add(GTK_CONTAINER(new_window), hbox);
    vbox = gtk_vbox_new(TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 5);

    output_frame = gtk_frame_new("Choose Your output file:");
	gtk_frame_set_shadow_type(GTK_FRAME(output_frame), GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(vbox), output_frame, TRUE, TRUE, 5);

	inner_vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(output_frame), inner_vbox);

    output_file_chooser = gtk_file_chooser_widget_new(GTK_FILE_CHOOSER_ACTION_SAVE);
    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(output_file_chooser), TRUE);
    gtk_box_pack_start(GTK_BOX(inner_vbox), output_file_chooser, TRUE, TRUE, 5);

    submit_button = gtk_button_new_with_label("Apply");
    gtk_box_pack_start(GTK_BOX(inner_vbox), submit_button, FALSE, FALSE, 5);

    sup_image = gtk_image_new_from_stock(GTK_STOCK_APPLY, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(submit_button), GTK_WIDGET(sup_image));
    gtk_button_set_image_position(GTK_BUTTON(submit_button), GTK_POS_LEFT);

    gpointer *args = new gpointer[3];
    args[0] = new_window;
    args[1] = outputArgs[1];
    args[2] = output_file_chooser;
    g_signal_connect(G_OBJECT(submit_button), "clicked", G_CALLBACK(output_saver), args);

    gtk_widget_show_all(new_window);
}
