#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

void bill() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return;
    }

    // ? Disable SSL
    unsigned int ssl_mode = SSL_MODE_DISABLED;
    mysql_options(conn, MYSQL_OPT_SSL_MODE, &ssl_mode);

    if (!mysql_real_connect(conn, "localhost", "root", "useradmin@100", "IPPROJECT", 0, NULL, 0)) {
        fprintf(stderr, "Connection failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    char name[100], number[20], query[512], item[100];
    int barcode_number, qty, db_qty, mrp, srp;
    
    while (getchar() != '\n');

    printf("                      |            Enter the Name of Customer: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    printf("                      |            Enter the Mobile Number: ");
    fgets(number, sizeof(number), stdin);
    number[strcspn(number, "\n")] = 0;
    printf("                      |            Enter the Barcode Number: ");
    scanf("%d", &barcode_number);
    while (getchar() != '\n');

    sprintf(query, "SELECT NAME_OF_ITEM, QUANTITY, MRP, SRP FROM STOCK WHERE BARCODE_NUMBER=%d", barcode_number);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) {
        printf("                      |            Item Not Found\n");
        mysql_free_result(res);
        mysql_close(conn);
        return;
    }

    row = mysql_fetch_row(res);
    strcpy(item, row[0]);
    db_qty = atoi(row[1]);
    mrp = atoi(row[2]);
    srp = atoi(row[3]);

    printf("                      |            NAME OF ITEM: %s\n", item);
    printf("                      |            QUANTITY: %d\n", db_qty);
    printf("                      |            MRP: %d\n", mrp);
    printf("                      |            SRP: %d\n", srp);

    printf("                      |            Enter the quantity required: ");
    scanf("%d", &qty);

    double tax = srp * 0.18;
    double total = (srp + tax) * qty;

    printf("                      |----------------------------------------------------------------------|\n");
    printf("                      |                                 Bill                                 |\n");
    printf("                      |----------------------------------------------------------------------|\n");
    printf("                      |[ Buyer's Name - %s ] [ Contact Number - %s ]\n", name, number);
    printf("                                             Name of item - %s\n", item);
    printf("                                             Quantity - %d\n", qty);
    printf("                                             Price - %d/-\n", srp);
    printf("                                             Tax - %.2f/-\n", tax);
    printf("                                             Total - %.2f/- ONLY\n", total);

    // Update stock
    int new_qty = db_qty - qty;
    sprintf(query, "UPDATE STOCK SET QUANTITY=%d WHERE BARCODE_NUMBER=%d", new_qty, barcode_number);
    mysql_query(conn, query);

    // Save to CSV
    FILE *f = fopen("output.csv", "a");
    if (f != NULL) {
        fprintf(f, "Name,Mobile Number,Item,Quantity,Price,Tax,Total\n");
        fprintf(f, "%s,%s,%s,%d,%d,%.2f,%.2f\n", name, number, item, qty, srp, tax, total);
        fclose(f);
    }

    printf("                      |                Data exported to CSV file successfully.               |\n");

    mysql_free_result(res);
    mysql_close(conn);
}

