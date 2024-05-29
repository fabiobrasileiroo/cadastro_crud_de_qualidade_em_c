#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ORDERS 100
#define MAX_PRODUCTS 1

typedef struct {
    char Name[50];
    float Price;
    int Quantity;
} Product;

typedef struct {
    int Id;
    Product Products[MAX_PRODUCTS];
    float Total;
} Order;

Order orders[MAX_ORDERS];
int numOrders = 0;

void saveToFile(const Order *orders, int numOrders, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    for (int i = 0; i < numOrders; i++) {
        fprintf(file, "Order ID: %d\n", orders[i].Id);
        for (int j = 0; j < MAX_PRODUCTS; j++) {
            if (orders[i].Products[j].Quantity == 0) break;
            fprintf(file, "Product #%d: %s - Price: %.2f - Quantity: %d\n", j + 1, orders[i].Products[j].Name, orders[i].Products[j].Price, orders[i].Products[j].Quantity);
        }
        fprintf(file, "Total: %.2f\n\n", orders[i].Total);
    }
    fclose(file);
}

void on_create_order_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *grid;
    GtkWidget *entry_name[MAX_PRODUCTS], *entry_price[MAX_PRODUCTS], *entry_quantity[MAX_PRODUCTS];
    gint response;
    Order *orders = (Order *)data;

    dialog = gtk_dialog_new_with_buttons("Create New Order", GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "OK", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

    for (int i = 0; i < MAX_PRODUCTS; i++) {
        entry_name[i] = gtk_entry_new();
        entry_price[i] = gtk_entry_new();
        entry_quantity[i] = gtk_entry_new();

        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Product Name:"), 0, i * 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), entry_name[i], 1, i * 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Product Price:"), 0, i * 3 + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), entry_price[i], 1, i * 3 + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Product Quantity:"), 0, i * 3 + 2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), entry_quantity[i], 1, i * 3 + 2, 1, 1);
    }

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);
    response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_OK) {
        int id = numOrders;
        orders[id].Id = id;
        float total = 0;
        for (int i = 0; i < MAX_PRODUCTS; i++) {
            const char *name = gtk_entry_get_text(GTK_ENTRY(entry_name[i]));
            const char *price_text = gtk_entry_get_text(GTK_ENTRY(entry_price[i]));
            const char *quantity_text = gtk_entry_get_text(GTK_ENTRY(entry_quantity[i]));

            float price = atof(price_text);
            int quantity = atoi(quantity_text);

            if (quantity > 0) {
                Product product = {.Price = price, .Quantity = quantity};
                strcpy(product.Name, name);
                orders[id].Products[i] = product;
                total += price * quantity;
            }
        }
        orders[id].Total = total;
        numOrders++;
    }
    gtk_widget_destroy(dialog);
}

void on_save_orders_button_clicked(GtkWidget *widget, gpointer data) {
    saveToFile((Order *)data, numOrders, "database.txt");
}

static gboolean filter_func(GtkTreeModel *model, GtkTreeIter *iter, gpointer data) {
    const gchar *search_text = (const gchar *)data;
    gchar *product_name;
    gtk_tree_model_get(model, iter, 1, &product_name, -1);

    gboolean visible = (strstr(product_name, search_text) != NULL);
    g_free(product_name);

    return visible;
}

void on_search_entry_changed(GtkEntry *entry, gpointer user_data) {
    GtkTreeModelFilter *filter = GTK_TREE_MODEL_FILTER(user_data);
    const gchar *text = gtk_entry_get_text(entry);
    gtk_tree_model_filter_refilter(filter);
    g_object_set_data(G_OBJECT(filter), "search-text", (gpointer)text);
}

void on_display_orders_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *scrolled_window, *treeview, *search_entry;
    GtkListStore *store;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeModelFilter *filter;

    dialog = gtk_dialog_new_with_buttons("All Orders", GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "OK", GTK_RESPONSE_OK, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    search_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(content_area), search_entry, FALSE, FALSE, 0);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled_window, 400, 300);
    gtk_container_add(GTK_CONTAINER(content_area), scrolled_window);

    store = gtk_list_store_new(5, G_TYPE_INT, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_INT, G_TYPE_FLOAT);

    for (int i = 0; i < numOrders; i++) {
        for (int j = 0; j < MAX_PRODUCTS; j++) {
            if (orders[i].Products[j].Quantity == 0) break;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               0, orders[i].Id,
                               1, orders[i].Products[j].Name,
                               2, orders[i].Products[j].Price,
                               3, orders[i].Products[j].Quantity,
                               4, orders[i].Total,
                               -1);
        }
    }

    filter = GTK_TREE_MODEL_FILTER(gtk_tree_model_filter_new(GTK_TREE_MODEL(store), NULL));
    gtk_tree_model_filter_set_visible_func(filter, (GtkTreeModelFilterVisibleFunc)filter_func, (gpointer)gtk_entry_get_text(GTK_ENTRY(search_entry)), NULL);

    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(filter));
    g_object_unref(store);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Order ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Product Name", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Price", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Quantity", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Total", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    gtk_container_add(GTK_CONTAINER(scrolled_window), treeview);
    gtk_widget_show_all(dialog);

    g_signal_connect(search_entry, "changed", G_CALLBACK(on_search_entry_changed), filter);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_theme_light_activate(GtkWidget *widget, gpointer data) {
    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-application-prefer-dark-theme", FALSE, NULL);
}

void on_theme_dark_activate(GtkWidget *widget, gpointer data) {
    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);
}

gboolean validate_login(const char *username, const char *password) {
    return (strcmp(username, "admin") == 0 && strcmp(password, "123") == 0);
}

gboolean show_login_dialog(GtkWindow *parent) {
    GtkWidget *dialog, *content_area, *username_entry, *password_entry, *grid;
    gint response;
    gboolean success = FALSE;

    dialog = gtk_dialog_new_with_buttons("Login", parent, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "Login", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

    username_entry = gtk_entry_new();
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(password_entry), '*');

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Username:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), username_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Password:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 1, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
        const char *password = gtk_entry_get_text(GTK_ENTRY(password_entry));
        success = validate_login(username, password);
        if (!success) {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Invalid username or password");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    }
    gtk_widget_destroy(dialog);
    return success;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window, *vbox, *create_order_button, *save_orders_button, *display_orders_button, *menubar, *menu, *menuitem_theme, *menuitem_light, *menuitem_dark;

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

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem_dark);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem_light);
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
