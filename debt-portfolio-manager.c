/*
 * Debt Portfolio Manager
 * Created by: Heitor Girelli Alves
 *
 * Purpose:
 * A console-based personal finance application that manages
 * debt accounts using a dynamically allocated linked list.
 * Debt records are automatically maintained in descending
 * order of interest rate, allowing users to identify and
 * manage repayment priorities efficiently.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_LEN 30

struct debt {
    char company[NAME_LEN + 1];
    double balance;
    double minimum;
    double interest_rate;
    struct debt *next;
};

struct debt *add_to_list(struct debt *list);
struct debt *delete_highest_priority(struct debt *list);
void search_list(struct debt *list);
void print_list(struct debt *list);
void clear_list(struct debt *list);
int read_line(char str[], int n);

int main(void)
{
    char code;
    struct debt *debt_list = NULL;

    printf("====================================\n");
    printf("      Debt Portfolio Manager\n");
    printf("====================================\n");

    for (;;) {
        printf("\nMenu\n");
        printf("a - Add debt account\n");
        printf("d - Delete highest-priority debt\n");
        printf("s - Search for a debt account\n");
        printf("p - Display debt portfolio\n");
        printf("q - Quit\n");

        printf("\nEnter operation code: ");
        scanf(" %c", &code);

        while (getchar() != '\n')
            ;

        switch (code) {
            case 'a':
                debt_list = add_to_list(debt_list);
                break;

            case 'd':
                debt_list = delete_highest_priority(debt_list);
                break;

            case 's':
                search_list(debt_list);
                break;

            case 'p':
                print_list(debt_list);
                break;

            case 'q':
                clear_list(debt_list);
                printf("\nExiting Debt Portfolio Manager.\n");
                return 0;

            default:
                printf("Invalid operation code.\n");
        }
    }
}

struct debt *add_to_list(struct debt *list)
{
    struct debt *new_debt;
    struct debt *current;

    new_debt = malloc(sizeof(struct debt));

    if (new_debt == NULL) {
        printf("Unable to allocate memory.\n");
        return list;
    }

    printf("\nDebt Account Name: ");
    read_line(new_debt->company, NAME_LEN);

    printf("Outstanding Balance ($): ");
    scanf("%lf", &new_debt->balance);

    printf("Monthly Minimum Payment ($): ");
    scanf("%lf", &new_debt->minimum);

    printf("Annual Interest Rate (%%): ");
    scanf("%lf", &new_debt->interest_rate);

    while (getchar() != '\n')
        ;

    new_debt->next = NULL;

    /* Insert at beginning if list is empty or rate is highest */
    if (list == NULL ||
        new_debt->interest_rate > list->interest_rate) {

        new_debt->next = list;

        printf("Debt account added successfully.\n");
        return new_debt;
    }

    current = list;

    while (current->next != NULL &&
           new_debt->interest_rate <= current->next->interest_rate) {
        current = current->next;
    }

    new_debt->next = current->next;
    current->next = new_debt;

    printf("Debt account added successfully.\n");

    return list;
}

struct debt *delete_highest_priority(struct debt *list)
{
    struct debt *new_head;

    if (list == NULL) {
        printf("No debt accounts found.\n");
        return NULL;
    }

    new_head = list->next;

    printf("Removed debt account: %s\n", list->company);

    free(list);

    return new_head;
}

void search_list(struct debt *list)
{
    char company_name[NAME_LEN + 1];
    struct debt *current;
    int found = 0;

    if (list == NULL) {
        printf("No debt accounts found.\n");
        return;
    }

    printf("Enter company name: ");
    read_line(company_name, NAME_LEN);

    for (current = list; current != NULL; current = current->next) {

        if (strcmp(current->company, company_name) == 0) {

            printf("\nDebt Account Found\n");
            printf("-----------------------------------------------\n");
            printf("Company      : %s\n", current->company);
            printf("Balance      : $%.2f\n", current->balance);
            printf("Min Payment  : $%.2f\n", current->minimum);
            printf("Interest Rate: %.2f%%\n",
                   current->interest_rate);

            found = 1;
            break;
        }
    }

    if (!found)
        printf("No matching debt account found.\n");
}

void print_list(struct debt *list)
{
    struct debt *current;

    if (list == NULL) {
        printf("No debt accounts found.\n");
        return;
    }

    printf("\nDebt Portfolio\n");
    printf("-----------------------------------------------------------------\n");
    printf("%-30s %10s %10s %10s\n",
           "Company",
           "Balance",
           "Minimum",
           "Rate (%)");
    printf("-----------------------------------------------------------------\n");

    for (current = list;
         current != NULL;
         current = current->next) {

        printf("%-30s %10.2f %10.2f %10.2f\n",
               current->company,
               current->balance,
               current->minimum,
               current->interest_rate);
    }
}

void clear_list(struct debt *list)
{
    struct debt *temp;

    while (list != NULL) {
        temp = list;
        list = list->next;
        free(temp);
    }
}

int read_line(char str[], int n)
{
    int ch;
    int i = 0;

    while ((ch = getchar()) != '\n' &&
           isspace(ch))
        ;

    if (ch == '\n') {
        str[0] = '\0';
        return 0;
    }

    str[i++] = ch;

    while ((ch = getchar()) != '\n') {
        if (i < n)
            str[i++] = ch;
    }

    str[i] = '\0';

    return i;
}