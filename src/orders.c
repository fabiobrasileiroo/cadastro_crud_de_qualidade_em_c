#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "orders.h"

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
