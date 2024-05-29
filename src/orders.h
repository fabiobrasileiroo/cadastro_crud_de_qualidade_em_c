#ifndef ORDERS_H
#define ORDERS_H

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

extern Order orders[MAX_ORDERS];
extern int numOrders;

void saveToFile(const Order *orders, int numOrders, const char *filename);
void on_create_order_button_clicked(GtkWidget *widget, gpointer data);
void on_save_orders_button_clicked(GtkWidget *widget, gpointer data);

#endif
