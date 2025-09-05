#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

void entry() {
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

    printf("                      |----------------------------------------------------------------------|\n");
    printf("                      |                              DATA ENTRY                              |\n");
    printf("                      |----------------------------------------------------------------------|\n");

    int barcode_number, qty, mrp, srp;
    char name_item[100];
    char query[512];

    printf("                      |            Enter the BARCODE NUMBER: ");
    scanf("%d", &barcode_number);
    while (getchar() != '\n');
    printf("                      |            Enter the NAME OF ITEM: ");
    fgets(name_item, sizeof(name_item), stdin);
    name_item[strcspn(name_item, "\n")] = 0;
    printf("                      |            Enter the QUANTITY: ");
    scanf("%d", &qty);
    while (getchar() != '\n');
    printf("                      |            Enter the MRP: ");
    scanf("%d", &mrp);
    while (getchar() != '\n');
    printf("                      |            Enter the SRP: ");
    scanf("%d", &srp);
    while (getchar() != '\n');

    sprintf(query, "SELECT QUANTITY FROM STOCK WHERE BARCODE_NUMBER=%d", barcode_number);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    res = mysql_store_result(conn);
    if (mysql_num_rows(res) > 0) {
        row = mysql_fetch_row(res);
        int old_qty = atoi(row[0]);
        int new_qty = old_qty + qty;
        sprintf(query, "UPDATE STOCK SET QUANTITY=%d WHERE BARCODE_NUMBER=%d", new_qty, barcode_number);
        mysql_query(conn, query);
        printf("                      |                              Item Updated                            |\n");
    } else {
        sprintf(query, "INSERT INTO STOCK (BARCODE_NUMBER, NAME_OF_ITEM, QUANTITY, MRP, SRP) VALUES (%d, '%s', %d, %d, %d)",
                barcode_number, name_item, qty, mrp, srp);
        mysql_query(conn, query);
        printf("                      |                          Item added to stock                         |\n");
    }

    mysql_free_result(res);
    mysql_close(conn);

    printf("                      |----------------------------------------------------------------------|\n");
}

