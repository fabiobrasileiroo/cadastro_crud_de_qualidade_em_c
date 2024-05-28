#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ORDERS 100
#define MAX_PRODUCTS 2

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
    FILE *file = fopen(filename, "a"); // Open in append mode
    if(file == NULL) {
        printf("Error opening file\n");
        return;
    }
    for(int i = 0; i < numOrders; i++) {
        fprintf(file, "Order ID: %d\n", orders[i].Id);
        for(int j = 0; j < MAX_PRODUCTS; j++) {
            if(orders[i].Products[j].Quantity == 0) {
                break;
            }
            fprintf(file, "Product #%d: %s - Price: %.2f - Quantity: %d\n", j + 1, orders[i].Products[j].Name, orders[i].Products[j].Price, orders[i].Products[j].Quantity);
        }
        fprintf(file, "Total: %.2f\n\n", orders[i].Total);
    }
    fclose(file);
}

void on_create_order_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area;
    GtkWidget *entry_name[MAX_PRODUCTS], *entry_price[MAX_PRODUCTS], *entry_quantity[MAX_PRODUCTS];
    GtkWidget *grid;
    gint response;
    Order *orders = (Order *)data;

    dialog = gtk_dialog_new_with_buttons("Create New Order", GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "OK", GTK_RESPONSE_OK,
                                         "Cancel", GTK_RESPONSE_CANCEL,
                                         NULL);

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

void on_display_orders_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area;
    GtkWidget *label;
    Order *orders = (Order *)data;
    char *orders_text = malloc(5000);
    orders_text[0] = '\0';

    for (int i = 0; i < numOrders; i++) {
        char order_text[500];
        snprintf(order_text, sizeof(order_text), "Order ID: %d\n", orders[i].Id);
        strcat(orders_text, order_text);

        for (int j = 0; j < MAX_PRODUCTS; j++) {
            if (orders[i].Products[j].Quantity == 0) {
                break;
            }
            snprintf(order_text, sizeof(order_text), "  Product #%d: %s - Price: %.2f - Quantity: %d\n", j + 1, orders[i].Products[j].Name, orders[i].Products[j].Price, orders[i].Products[j].Quantity);
            strcat(orders_text, order_text);
        }
        snprintf(order_text, sizeof(order_text), "  Total: %.2f\n\n", orders[i].Total);
        strcat(orders_text, order_text);
    }

    dialog = gtk_dialog_new_with_buttons("All Orders", GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "OK", GTK_RESPONSE_OK,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    label = gtk_label_new(orders_text);
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show_all(dialog);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    free(orders_text);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *create_order_button;
    GtkWidget *save_orders_button;
    GtkWidget *display_orders_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Order Management System");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

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
