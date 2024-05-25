#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ORDERS 100
#define MAX_PRODUCTS 5

typedef struct {
    char Name[50];
    float Price;
    int Quantity;
}Product;

typedef struct {
    int Id;
    Product Products[MAX_PRODUCTS];
    float Total;
}Order;

void createTable(int numOrders) {
    printf("| Order ID |");
    for(int i = 0; i < MAX_PRODUCTS; i++) {
        printf(" Product #%d |", i + 1);
    }
    printf(" Total |\n");
    for(int i = 0; i < numOrders; i++) {
        printf("|----------|");
        for(int j = 0; j < MAX_PRODUCTS; j++) {
            printf("----------|");
        }
        printf("-----------|\n");
    }
}

void saveToFile(const Order *orders, int numOrders, const char *filename) {
    FILE *file = fopen(filename, "w");
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

int main() {
    int numOrders;
    printf("Enter the number of orders: ");
    scanf("%d", &numOrders);

    Order orders[numOrders];
    int id = 0;

    while(id < numOrders) {
        printf("\e[1;1H\e[2J");
        createTable(numOrders);

        orders[id].Id = id;
        printf("\nOrder ID: %d", id + 1);

        float total = 0;

        for(int i = 0; i < MAX_PRODUCTS; i++){
            while(getchar() != '\n');

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
        id++;

        printf("\nPress any key to continue...");
        while(getchar() != '\n');
    }

    saveToFile(orders, numOrders, "database.txt");

    return 0;
}

