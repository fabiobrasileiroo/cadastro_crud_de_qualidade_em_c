#include <gtk/gtk.h>
#include "orders.h"
#include "display_orders.h"
#include "login.h"

Order orders[MAX_ORDERS];
int numOrders = 0;

void on_theme_light_activate(GtkWidget *widget, gpointer data) {
    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-application-prefer-dark-theme", FALSE, NULL);
}

void on_theme_dark_activate(GtkWidget *widget, gpointer data) {
    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *create_order_button;
    GtkWidget *save_orders_button;
    GtkWidget *display_orders_button;
    GtkWidget *menubar;
    GtkWidget *menu;
    GtkWidget *menuitem_theme;
    GtkWidget *menuitem_light;
    GtkWidget *menuitem_dark;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Order Management System - Fabinho");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    if (!show_login_dialog(GTK_WINDOW(window))) {
        return 0;
    }

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    menubar = gtk_menu_bar_new();
    menu = gtk_menu_new();
    menuitem_theme = gtk_menu_item_new_with_label("Theme");
    menuitem_light = gtk_menu_item_new_with_label("Light");
    menuitem_dark = gtk_menu_item_new_with_label("Dark");

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem_light);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem_dark);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem_theme), menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem_theme);

    g_signal_connect(menuitem_light, "activate", G_CALLBACK(on_theme_light_activate), NULL);
    g_signal_connect(menuitem_dark, "activate", G_CALLBACK(on_theme_dark_activate), NULL);

    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    create_order_button = gtk_button_new_with_label("Create New Order");
    g_signal_connect(create_order_button, "clicked", G_CALLBACK(on_create_order_button_clicked), orders);
    gtk_box_pack_start(GTK_BOX(vbox), create_order_button, TRUE, TRUE, 0);

    save_orders_button = gtk_button_new_with_label("Save Orders to File");
    g_signal_connect(save_orders_button, "clicked", G_CALLBACK(on_save_orders_button_clicked), orders);
    gtk_box_pack_start(GTK_BOX(vbox), save_orders_button, TRUE, TRUE, 0);

    display_orders_button = gtk_button_new_with_label("Display All Orders");
    g_signal_connect(display_orders_button, "clicked", G_CALLBACK(on_display_orders_button_clicked), orders);
    gtk_box_pack_start(GTK_BOX(vbox), display_orders_button, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
