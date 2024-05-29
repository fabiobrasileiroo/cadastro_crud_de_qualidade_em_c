#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "orders.h"
#include "display_orders.h"

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
    g_object_set_data(G_OBJECT(filter), "search-text", g_strdup(text));
}

void on_display_orders_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *scrolled_window, *treeview, *entry;
    GtkListStore *store;
    GtkTreeModelFilter *filter;
    GtkTreeIter iter;
    Order *orders = (Order *)data;

    dialog = gtk_dialog_new_with_buttons("All Orders", GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "OK", GTK_RESPONSE_OK, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    store = gtk_list_store_new(5, G_TYPE_INT, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_INT, G_TYPE_FLOAT);

    for (int i = 0; i < numOrders; i++) {
        for (int j = 0; j < MAX_PRODUCTS; j++) {
            if (orders[i].Products[j].Quantity == 0) break;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, orders[i].Id, 1, orders[i].Products[j].Name, 2, orders[i].Products[j].Price, 3, orders[i].Products[j].Quantity, 4, orders[i].Total, -1);
        }
    }

    filter = GTK_TREE_MODEL_FILTER(gtk_tree_model_filter_new(GTK_TREE_MODEL(store), NULL));
    gtk_tree_model_filter_set_visible_func(filter, filter_func, g_strdup(""), g_free);

    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(filter));
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Order ID", gtk_cell_renderer_text_new(), "text", 0, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Product Name", gtk_cell_renderer_text_new(), "text", 1, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Price", gtk_cell_renderer_text_new(), "text", 2, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Quantity", gtk_cell_renderer_text_new(), "text", 3, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Total", gtk_cell_renderer_text_new(), "text", 4, NULL);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), treeview);

    entry = gtk_entry_new();
    g_signal_connect(entry, "changed", G_CALLBACK(on_search_entry_changed), filter);
    g_object_set_data(G_OBJECT(filter), "search-text", g_strdup(""));

    gtk_box_pack_start(GTK_BOX(content_area), entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), scrolled_window, TRUE, TRUE, 0);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
