#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 100

struct Donor {
    char name[100];
    char blood_group[10];
    int age;
    char gender[10];
    char contact[15];
    char city[50];
};

// Convert string to lowercase for case-insensitive comparison
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

// Load donors from file
int load_donors(struct Donor donors[]) {
    FILE *fp = fopen("bloodbank.txt", "r");
    if (!fp)
        return 0;

    int count = 0;
    while (fscanf(fp, "%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^\n]\n",
                  donors[count].name,
                  donors[count].blood_group,
                  &donors[count].age,
                  donors[count].gender,
                  donors[count].contact,
                  donors[count].city) == 6) {
        count++;
    }
    fclose(fp);
    return count;
}

// Save donors back to file
void save_donors(struct Donor donors[], int count) {
    FILE *fp = fopen("bloodbank.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s|%s|%d|%s|%s|%s\n",
                donors[i].name,
                donors[i].blood_group,
                donors[i].age,
                donors[i].gender,
                donors[i].contact,
                donors[i].city);
    }
    fclose(fp);
}

// Add a new donor
void add_donor() {
    struct Donor donors[MAX];
    int count = load_donors(donors);

    struct Donor new_donor;

    printf("Enter donor name: ");
    fgets(new_donor.name, sizeof(new_donor.name), stdin);
    new_donor.name[strcspn(new_donor.name, "\n")] = '\0';

    printf("Enter blood group (A+, A-, B+, B-, O+, O-, AB+, AB-): ");
    fgets(new_donor.blood_group, sizeof(new_donor.blood_group), stdin);
    new_donor.blood_group[strcspn(new_donor.blood_group, "\n")] = '\0';

    printf("Enter age: ");
    scanf("%d", &new_donor.age);
    getchar();

    printf("Enter gender (Male/Female): ");
    fgets(new_donor.gender, sizeof(new_donor.gender), stdin);
    new_donor.gender[strcspn(new_donor.gender, "\n")] = '\0';

    printf("Enter contact number: ");
    fgets(new_donor.contact, sizeof(new_donor.contact), stdin);
    new_donor.contact[strcspn(new_donor.contact, "\n")] = '\0';

    printf("Enter city: ");
    fgets(new_donor.city, sizeof(new_donor.city), stdin);
    new_donor.city[strcspn(new_donor.city, "\n")] = '\0';

    donors[count++] = new_donor;

    save_donors(donors, count);
    printf("\n Donor added successfully!\n");
}

// Display all donors
void display_donors() {
    struct Donor donors[MAX];
    int count = load_donors(donors);

    if (count == 0) {
        printf("\n(No donor records found)\n");
        return;
    }

    printf("\n%-3s %-25s %-10s %-5s %-10s %-15s %-15s\n", "#", "Name", "BloodGrp", "Age", "Gender", "Contact", "City");
    printf("------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-3d %-25s %-10s %-5d %-10s %-15s %-15s\n",
               i + 1,
               donors[i].name,
               donors[i].blood_group,
               donors[i].age,
               donors[i].gender,
               donors[i].contact,
               donors[i].city);
    }
}

// Search donor by name or blood group
void search_donor() {
    struct Donor donors[MAX];
    int count = load_donors(donors);
    if (count == 0) {
        printf("\n(No donor records found)\n");
        return;
    }

    char search[50];
    printf("Enter donor name or blood group to search: ");
    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = '\0';

    char lower_search[50];
    strcpy(lower_search, search);
    to_lowercase(lower_search);

    printf("\n%-3s %-25s %-10s %-5s %-10s %-15s %-15s\n", "#", "Name", "BloodGrp", "Age", "Gender", "Contact", "City");
    printf("------------------------------------------------------------------------------------------\n");

    int found = 0;
    for (int i = 0; i < count; i++) {
        char lower_name[100], lower_bg[10];
        strcpy(lower_name, donors[i].name);
        strcpy(lower_bg, donors[i].blood_group);
        to_lowercase(lower_name);
        to_lowercase(lower_bg);

        if (strstr(lower_name, lower_search) || strstr(lower_bg, lower_search)) {
            found = 1;
            printf("%-3d %-25s %-10s %-5d %-10s %-15s %-15s\n",
                   i + 1,
                   donors[i].name,
                   donors[i].blood_group,
                   donors[i].age,
                   donors[i].gender,
                   donors[i].contact,
                   donors[i].city);
        }
    }

    if (!found)
        printf("\nNo matching donor found.\n");
}

// Update donor details
void update_donor() {
    struct Donor donors[MAX];
    int count = load_donors(donors);

    if (count == 0) {
        printf("\n(No donor records found)\n");
        return;
    }

    display_donors();

    int choice;
    printf("\nEnter donor number to update: ");
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > count) {
        printf("Invalid choice.\n");
        return;
    }

    struct Donor *d = &donors[choice - 1];

    printf("Enter new name (leave blank to keep '%s'): ", d->name);
    char input[100];
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = '\0';
        strcpy(d->name, input);
    }

    printf("Enter new blood group (leave blank to keep '%s'): ", d->blood_group);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = '\0';
        strcpy(d->blood_group, input);
    }

    printf("Enter new age (0 to keep %d): ", d->age);
    int age;
    scanf("%d", &age);
    getchar();
    if (age != 0) d->age = age;

    printf("Enter new gender (leave blank to keep '%s'): ", d->gender);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = '\0';
        strcpy(d->gender, input);
    }

    printf("Enter new contact (leave blank to keep '%s'): ", d->contact);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = '\0';
        strcpy(d->contact, input);
    }

    printf("Enter new city (leave blank to keep '%s'): ", d->city);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = '\0';
        strcpy(d->city, input);
    }

    save_donors(donors, count);
    printf("\n✅ Donor updated successfully!\n");
}

// Delete donor record
void delete_donor() {
    struct Donor donors[MAX];
    int count = load_donors(donors);

    if (count == 0) {
        printf("\n(No donor records found)\n");
        return;
    }

    display_donors();

    int choice;
    printf("\nEnter donor number to delete: ");
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > count) {
        printf("Invalid choice.\n");
        return;
    }

    for (int i = choice - 1; i < count - 1; i++) {
        donors[i] = donors[i + 1];
    }
    count--;

    save_donors(donors, count);
    printf("\n🗑 Donor deleted successfully!\n");
}

int main() {
    int option;
    do {
        printf("\n===== BLOOD BANK MANAGEMENT SYSTEM =====\n");
        printf("1. Add Donor\n");
        printf("2. Display All Donors\n");
        printf("3. Search Donor\n");
        printf("4. Update Donor\n");
        printf("5. Delete Donor\n");
        printf("0. Exit\n");
        printf("Choose option: ");
        scanf("%d", &option);
        getchar();

        switch (option) {
            case 1: add_donor(); break;
            case 2: display_donors(); break;
            case 3: search_donor(); break;
            case 4: update_donor(); break;
            case 5: delete_donor(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid option!\n");
        }
    } while (option != 0);

    return 0;
}
