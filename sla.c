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

void createNewOrder(GtkWidget *widget, gpointer data) {
    Order *orders = (Order *)data;

    int id = numOrders;
    orders[id].Id = id;
    printf("\nOrder ID: %d\n", id + 1);

    float total = 0;

    for(int i = 0; i < MAX_PRODUCTS; i++) {
        char name[50];
        float price;
        int quantity;

        printf("\nEnter the name of the product: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;

        printf("\nEnter the price of the product: ");
        scanf("%f", &price);

        printf("\nEnter the quantity of the product: ");
        scanf("%d", &quantity);

        if (i < MAX_PRODUCTS) {
            Product product = {.Price = price, .Quantity = quantity};
            strcpy(product.Name, name);

            orders[id].Products[i] = product;
            total += price * quantity;
        }

        printf("\nDo you want to add another product? [y/n] ");
        char newProduct;
        scanf(" %c", &newProduct);

        if (newProduct != 'y') {
            break;
        }
    }
    orders[id].Total = total;
    numOrders++;
}

void displayOrders(GtkWidget *widget, gpointer data) {
    Order *orders = (Order *)data;

    printf("\nAll Orders:\n");
    for(int i = 0; i < numOrders; i++) {
        printf("Order ID: %d\n", orders[i].Id);
        for(int j = 0; j < MAX_PRODUCTS; j++) {
            if(orders[i].Products[j].Quantity == 0) {
                break;
            }
            printf("  Product #%d: %s - Price: %.2f - Quantity: %d\n", j + 1, orders[i].Products[j].Name, orders[i].Products[j].Price, orders[i].Products[j].Quantity);
        }
        printf("  Total: %.2f\n\n", orders[i].Total);
    }
}

void on_create_order_button_clicked(GtkWidget *widget, gpointer data) {
    createNewOrder(widget, data);
}

void on_save_orders_button_clicked(GtkWidget *widget, gpointer data) {
    saveToFile((Order *)data, numOrders, "database.txt");
}

void on_display_orders_button_clicked(GtkWidget *widget, gpointer data) {
    displayOrders(widget, data);
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

    vbox = gtk_vbox_new(FALSE, 5);
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
