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

void createTable(int numOrders) {
    printf("| Order ID |");
    for(int i = 0; i < MAX_PRODUCTS; i++) {
        printf(" Product #%d |", i + 1);
    }
    printf(" Total |\n");
    for(int i = 0; i < numOrders; i++) {
        printf("|----------|");
        for(int j = 0; j < MAX_PRODUCTS; j++) {
            printf("------------|");
        }
        printf("-----------|\n");
    }
}

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

void createNewOrder(Order *orders, int *numOrders) {
    int id = *numOrders;

    orders[id].Id = id;
    printf("\nOrder ID: %d\n", id + 1);

    float total = 0;

    for(int i = 0; i < MAX_PRODUCTS; i++){
        while(getchar() != '\n'); // Clear input buffer

        printf("\nEnter the name of the product: ");
        char name[50];
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;

        printf("\nEnter the price of the product: ");
        float price;
        scanf("%f", &price);

        printf("\nEnter the quantity of the product: ");
        int quantity;
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

        if (newProduct != 'y'){
            break;
        }
    }
    orders[id].Total = total;
    (*numOrders)++;
}

void displayOrders(const Order *orders, int numOrders) {
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

int main() {
    int numOrders = 0;
    Order orders[MAX_ORDERS];
    char filename[] = "database.txt";

    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Create new order\n");
        printf("2. Save orders to file\n");
        printf("3. Display all orders\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createNewOrder(orders, &numOrders);
                break;
            case 2:
                saveToFile(orders, numOrders, filename);
                printf("Orders saved to %s\n", filename);
                break;
            case 3:
                displayOrders(orders, numOrders);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}
