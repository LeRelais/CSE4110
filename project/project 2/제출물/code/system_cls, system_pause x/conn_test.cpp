#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "mysql";
const char* db = "project";

int mainchoice, flag = 1;
enum menu_type { QUIT, I, II, III, IV, V };

void type5menu() {
	//system("cls");
	printf("----- TYPE V -----\n\n");
	printf("** Generate the bill for each customer for the past month. **\n");
}

void type4menu() {
	//system("cls");
	printf("----- TYPE IV -----\n\n");
	printf("** Find the packages that were not delivered within the promised time. **\n\n");

}

void type3menu() {
	//system("cls");
	printf("----- TYPE III -----\n\n");
	printf("** Find the customer who has spent the most money on shipping in the past year. **\n");
	printf("Which Year? : ");
}

void type2menu() {
	//system("cls");
	printf("----- TYPE II -----\n\n");
	printf("** Find the customer who has shipped the most packages in the past year. **\n");
	printf("Which Year? : ");
}

void type1_3submenu() {
	//system("cls");
	printf("----- TYPE I-3 -----\n");
	printf("** Find the last successful delivery by that truck prior to the crash. **\n");
}

void type1_2submenu() {
	//system("cls");
	printf("----- TYPE I-2 -----\n");
	printf("** Find all recipients who had a package on that truck at the time of the crash. **\n");
}

void type1_1submenu() {
	//system("cls");
	printf("----- TYPE I-1 -----\n");
	printf("** Find all customers who had a package on the truck at the time of the crash. **\n");
}

void type1menu() {
	//system("cls");
	printf("----- Subtypes in TYPE I -----\n");
	printf("    1. TYPE I-1.\n");
	printf("    2. TYPE I-2.\n");
	printf("    3. TYPE I-3.\n");
	printf("    Choice : ");
}

void mainmenu() {
	//system("cls");
	printf("------ SELECT QUERY TYPES ------\n\n");
	printf("       1. TYPE I\n");
	printf("       2. TYPE II\n");
	printf("       3. TYPE III\n");
	printf("       4. TYPE IV\n");
	printf("       5. TYPE V\n");
	printf("       0. QUIT\n");
	printf("       Choose : ");
}

int main(void) {

	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		printf("Connection Succeed\n");

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}
		FILE* f = fopen("create.txt", "r");
		//const char* query = "create table customer (security_number char(13), customer_name varchar(18), phone_number char(11), primary key (security_number))";
		char rline[4000];
		const char* query = "";
		//printf("1\n");
		while (fgets(rline, 4000, f)) {
			//printf("%d\n", strlen(rline));
			int state;
			rline[strlen(rline) - 1] = '\0';
			//printf("%s\n", rline);
			mysql_query(connection, rline);
		}
		fclose(f);
		f = fopen("insert.txt", "r");
		while (fgets(rline, 4000, f)) {
			rline[strlen(rline) - 1] = '\0';
			mysql_query(connection, rline);
		}
		fclose(f);
		/*int state = 0;

		state = mysql_query(connection, query);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("%s %s %s\n", sql_row[0], sql_row[1], sql_row[2]);
			}
			mysql_free_result(sql_result);
		}*/
		int flag = 1, flagquit = 1;
		while (1) {
			mainmenu();
			flag = 1;
			scanf("%d", &mainchoice);
			switch (mainchoice) {
			case I:
				while (1) {
					int menuchoice = -1;
					if (flag == 0)
						break;
					//type1menu();            //type1 menu 출력
					//system("pause");
					while (menuchoice < 0 || menuchoice > 3) {
						type1menu();
						scanf("%d", &menuchoice);
						//system("cls");

					}
					if (menuchoice == 0)    //입력이 0이면 mainmenu 출력
						break;
					else {
						if (menuchoice == 1) {   //아니면 suboption menu 출력 
							type1_1submenu();
							int num;
							printf("Which Truck? : ");
							scanf("%d", &num);
							if (num != 0) {             //입력으로 받은 숫자가 0이면 다시 type1 menu로 돌아감
								sprintf(rline, "select distinct c.security_number, c.customer_name from customer c right outer join package p on c.security_number = p.security_number right outer join shipment s on p.shipment_id = s.shipment_id where s.shipment_id in (select shipment_id from transportation t where transportation_id = '%d' and transportation_status = 'crash')", num);
								int state;
								//strcpy(rline, "select * from customer c right outer join package p on c.security_number = p.security_number right outer join shipment s on p.shipment_id = s.shipment_id where s.shipment_id in (select shipment_id from transportation t where transportation_id = '7768' and transportation_status = 'crash')");
								state = mysql_query(connection, rline);
								if (state == 0)
								{
									sql_result = mysql_store_result(connection);
									while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
									{
										printf("%s %s\n", sql_row[0], sql_row[1]);
									}
									mysql_free_result(sql_result);
								}

								flag = 0;
								//system("pause");
							}
						}
						else if (menuchoice == 2) {
							type1_2submenu();
							int num;
							printf("Which Truck? : ");
							scanf("%d", &num);
							if (num != 0) {               //입력으로 받은 숫자가 0이면 다시 type1 menu로 돌아감
								sprintf(rline, "select distinct r.recipient_id from recipient r right outer join package p on r.recipient_id = p.recipient_id right outer join shipment s on p.shipment_id = s.shipment_id where s.shipment_id in (select shipment_id from transportation t where transportation_id = '%d' and transportation_status = 'crash')", num);
								int state;
								state = mysql_query(connection, rline);
								if (state == 0)
								{
									sql_result = mysql_store_result(connection);
									while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
									{
										printf("%s\n", sql_row[0]);
									}
									mysql_free_result(sql_result);
								}

								flag = 0;
								//system("pause");
							}

						}
						else if (menuchoice == 3) {
							type1_3submenu();
							int num;
							printf("Which Truck? : ");
							scanf("%d", &num);
							if (num != 0) {               //입력으로 받은 숫자가 0이면 다시 type1 menu로 돌아감

								int state;
								sprintf(rline, "select distinct ti.shipment_id, ti.actual_arrival_timedate from delivery d join transportation t on d.transportation_id = t.transportation_id join delivery_time ti on d.shipment_id = ti.shipment_id where (t.transportation_status != 'crash' and t.transportation_id = '%d') order by ti.actual_arrival_timedate desc limit 1", num);

								state = mysql_query(connection, rline);
								if (state == 0)
								{
									sql_result = mysql_store_result(connection);
									while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
									{
										printf("%s %s\n", sql_row[0], sql_row[1]);
									}
									mysql_free_result(sql_result);
								}

								flag = 0;  //0이 아니었다면 query 처리 후 main menu로 복귀 
								//system("pause");
							}

						}
					}
				}
				break;
			case II:
				int menuchoice;
				type2menu();
				scanf("%d", &menuchoice);
				if (menuchoice > 0) {
					sprintf(rline, "select c.customer_name, COUNT(*) as packagecount from customer c join package p on c.security_number = p.security_number join payment s on s.payment_id = p.payment_id where year(s.payment_date) = %d group by c.security_number order by packagecount desc limit 1", menuchoice);
					int state;
					state = mysql_query(connection, rline);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s %s\n", sql_row[0], sql_row[1]);
						}
						mysql_free_result(sql_result);
					}
					flag = 0;
					//system("pause");
				}
				break;
			case III:
				type3menu();
				scanf("%d", &menuchoice);
				if (menuchoice > 0) {
					sprintf(rline, "select c.customer_name, sum(s.payment_amount) as total_payment from customer c join customer_account ac on c.security_number = ac.security_number join payment s on ac.account_id = s.account_id where year(s.payment_date) = %d group by c.security_number order by total_payment desc limit 1", menuchoice);
					int state;
					state = mysql_query(connection, rline);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s %s\n", sql_row[0], sql_row[1]);
						}
						mysql_free_result(sql_result);
					}
					flag = 0;
					//system("pause");
				}
				break;
			case IV:
				type4menu();

				sprintf(rline, "select distinct package_id from package p join delivery_time t on p.shipment_id = t.shipment_id join delivery d on p.shipment_id = d.shipment_id where (d.transportation_status = 'crash' or t.actual_arrival_timedate > t.estimated_delivery_timedate)");
				int state;
				state = mysql_query(connection, rline);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s\n", sql_row[0]);
					}
					mysql_free_result(sql_result);
				}
				flag = 0;
				//system("pause");

				break;
			case V:
				type5menu();
				printf("Which Year? : ");
				int month;
				scanf("%d", &menuchoice);
				if (menuchoice > 0) {
					printf("Which Month? : ");
					scanf("%d", &month);
					if (month > 0) {
						sprintf(rline, "select p.account_id, sum(p.payment_amount) as bill from payment p where '%d' = year(p.payment_date) and '%d' = month(p.payment_date) group by p.account_id order by bill desc", menuchoice, month - 1);
						//sprintf(rline, "select ca.account_id, sum(p.payment_amount) from customer_account ca join payment p on ca.account_id = p.account_id join customer c on ca.security_number = c.security_number");
						int state;
						state = mysql_query(connection, rline);
						if (state == 0)
						{
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("%s %s\n", sql_row[0], sql_row[1]);
							}
							mysql_free_result(sql_result);
						}
						flag = 0;
						system("pause");
					}
				}
				break;
			case QUIT:
				flagquit = 0;
				break;
			}
			if (flagquit == 0)
				break;
		}
		f = fopen("deletetuple.txt", "r");
		while (fgets(rline, 4000, f)) {
			rline[strlen(rline) - 1] = '\0';
			mysql_query(connection, rline);
		}
		fclose(f);
		f = fopen("drop.txt", "r");
		while (fgets(rline, 4000, f)) {
			//printf("%d\n", strlen(rline));
			rline[strlen(rline) - 1] = '\0';
			mysql_query(connection, rline);
		}
		fclose(f);
		mysql_close(connection);
	}

	return 0;
}






/*

	printf("------- SELECT QUERY TYPES -------\n\n");
	printf("\t1. TYPE 1\n");
	printf("\t2. TYPE 2\n");
	printf("\t3. TYPE 3\n");
	printf("\t4. TYPE 4\n");
	printf("\t5. TYPE 5\n");
	printf("\t6. TYPE 6\n");
	printf("\t7. TYPE 7\n");
	printf("\t0. QUIT\n");
	//printf("----------------------------------\n");
	printf("\n\n");
	printf("---- TYPE 5 ----\n\n");
	printf("** Find the top k brands by unit sales by the year**\n");
	printf(" Which K? : 3\n");

	return 0;

}
*/