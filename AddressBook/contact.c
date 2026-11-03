#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

void listContacts(AddressBook *addressBook, int sortCriteria) 
{   
    // If sortCriteria isn't passed from main, ask the user interactively
    if (sortCriteria < 1 || sortCriteria > 3) {
        printf("Choose sorting criteria:\n");
        printf("1. Sort by name\n");
        printf("2. Sort by phone\n");
        printf("3. Sort by email\n");
        printf("Enter your choice: ");
        scanf("%d", &sortCriteria);
    }
    // Bubble Sort logic based on sortCriteria
    Contact temp;
    for (int i = 0; i < addressBook->contactCount - 1; i++) {
        for (int j = 0; j < addressBook->contactCount - i - 1; j++) {
            int swapNeeded = 0;

            if (sortCriteria == 1) {
                // Sort by Name
                if (strcasecmp(addressBook->contacts[j].name, addressBook->contacts[j + 1].name) > 0) {
                    swapNeeded = 1;
                }
            } 
            else if (sortCriteria == 2) {
                // Sort by Phone
                if (strcmp(addressBook->contacts[j].phone, addressBook->contacts[j + 1].phone) > 0) {
                    swapNeeded = 1;
                }
            } 
            else if (sortCriteria == 3) {
                // Sort by Email
                if (strcmp(addressBook->contacts[j].email, addressBook->contacts[j + 1].email) > 0) {
                    swapNeeded = 1;
                }
            }

            // Perform the swap if needed
            if (swapNeeded) {
                temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }

    // Your print code below the sorting
    printf("%-20s\t%-15s\t%-30s\n", "Name", "Phone", "Email_id");
    for(int i = 0; i < addressBook->contactCount; i++) {
        printf("%-20s\t%-15s\t%-30s\n", 
               addressBook->contacts[i].name, 
               addressBook->contacts[i].phone, 
               addressBook->contacts[i].email);
    }
}
void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    // populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}


void createContact(AddressBook *addressBook)
{
    char str[50];
    
    // Read the name from the user then store into database
    printf("Enter the name: ");
    scanf(" %[^\n]", str);
    strcpy(addressBook->contacts[addressBook->contactCount].name, str);
    
    // Read and validate the phone number
    while (1) {
        printf("Enter the phone number: ");
        scanf("%s", str);
        
        if (validate_phone_no(addressBook, str)) {
            break;
        }
        printf("Invalid phone number or already exists! Must be 10 digits and unique.\n");
    }
    strcpy(addressBook->contacts[addressBook->contactCount].phone, str);
    
    // Read and validate the email id
    while (1) {
        printf("Enter the email id: ");
        scanf("%s", str);
        
        if (validate_email(addressBook, str)) {
            break;
        }
        printf("Invalid email or already exists! Check lowercase, '@', '.', and format.\n");
    }
    strcpy(addressBook->contacts[addressBook->contactCount].email, str);
    
    // Increment the contact count + 1
    addressBook->contactCount++;
    
    printf("Contact successfully created!\n");
}

int searchContact(AddressBook *addressBook) 
{
    int choice;
    char query[50];
    int flag = 0; // Flag to track if contact is found

    // Print the menu
    printf("Search by:\n");
    printf("1. Search by name\n");
    printf("2. Search by phone no\n");
    printf("3. Search by email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter the name: ");
        scanf(" %[^\n]", query);
        
        printf("Index\tName\t\tPhone\t\tEmail_id\n");
        for (int i = 0; i < addressBook->contactCount; i++) {
            if (strcasestr(addressBook->contacts[i].name, query) != NULL) {
                // Using formatted widths (e.g., %-15s) to guarantee alignment
                printf("%-5d\t%-15s\t%-15s\t%-15s\n", 
                    i, 
                    addressBook->contacts[i].name, 
                    addressBook->contacts[i].phone, 
                    addressBook->contacts[i].email);
                flag = 1;
            }
        }
    }
    else if (choice == 2) {
        printf("Enter the phone number: ");
        scanf("%s", query);
        
        printf("Index\tName\t\tPhone\t\tEmail_id\n");
        for (int i = 0; i < addressBook->contactCount; i++) {
            if (strcasestr(addressBook->contacts[i].phone, query) != NULL) {
                printf("%d\t%s\t%s\t%s\n", i, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
                flag = 1;
            }
        }
    }
    else if (choice == 3) {
        printf("Enter the email id: ");
        scanf("%s", query);
        
        printf("Index\tName\t\tPhone\t\tEmail_id\n");
        for (int i = 0; i < addressBook->contactCount; i++) {
            if (strcasestr(addressBook->contacts[i].email, query) != NULL) {
                printf("%d\t%s\t%s\t%s\n", i, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
                flag = 1;
            }
        }
    }
    else {
        printf("Invalid choice!\n");
        return 0;
    }

    // If flag is still 0, no contact was found
    if (!flag) {
        printf("Contact not found.\n");
        return 0; // Return 0 for not found
    }

    return 1; // Return 1 for contact found
}

void editContact(AddressBook *addressBook)
{
    // Call search contact function and collect the return value
    int ret = searchContact(addressBook);
    
    // If ret == 0 then contact not found, cannot edit, return to main menu
    if (ret == 0) {
        return;
    }
    
    // Ask the user to enter the index they want to edit
    int index;
    printf("Enter the index you want to edit: ");
    scanf("%d", &index);
    
    // Validate that the index is within valid bounds
    if (index < 0 || index >= addressBook->contactCount) {
        printf("Invalid index!\n");
        return;
    }
    
    int choice;
    char str[50];
    
    // Print edit menu
    printf("Choose what to edit:\n");
    printf("1. Edit name\n");
    printf("2. Edit phone\n");
    printf("3. Edit email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        // Read the new name and overwrite the current name
        printf("Enter the new name: ");
        scanf(" %[^\n]", str);
        strcpy(addressBook->contacts[index].name, str);
        printf("Edit contact successful, back to main menu\n");
    }
    else if (choice == 2) {
        // Read the new phone number, validate it, and overwrite it
        while (1) {
            printf("Enter the new phone number: ");
            scanf("%s", str);
            
            if (validate_phone_no(addressBook, str)) {
                break;
            }
            printf("Invalid phone number or already exists! Must be 10 digits and unique.\n");
        }
        strcpy(addressBook->contacts[index].phone, str);
        printf("Edit contact successful, back to main menu\n");
    }
    else if (choice == 3) {
        // Read the new email id, validate it, and overwrite it
        while (1) {
            printf("Enter the new email id: ");
            scanf("%s", str);
            
            if (validate_email(addressBook, str)) {
                break;
            }
            printf("Invalid email or already exists! Check lowercase, '@', '.', and format.\n");
        }
        strcpy(addressBook->contacts[index].email, str);
        printf("Edit contact successful, back to main menu\n");
    }
    else {
        printf("Invalid choice!\n");
    }
}

void deleteContact(AddressBook *addressBook)
{
    // Call search contact function and collect the ret value[cite: 5]
    int ret = searchContact(addressBook);
    
    // If ret == 0 then contact not found, cannot delete the contact, back to main menu[cite: 5]
    if (ret == 0) {
        return;
    }
    
    // Ask the user to enter the index want to delete[cite: 5]
    int index;
    printf("Enter the index you want to delete: ");
    scanf("%d", &index);
    
    // Validate index bounds
    if (index < 0 || index >= addressBook->contactCount) {
        printf("Invalid index!\n");
        return;
    }
    
    // Ask sure you want to delete the index or not y/n[cite: 5]
    char confirm;
    printf("Are you sure you want to delete index %d? (y/n): ", index);
    scanf(" %c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        // From the entered index to last index of contacts structure need to shift[cite: 5]
        for (int i = index; i < addressBook->contactCount - 1; i++) {
            addressBook->contacts[i] = addressBook->contacts[i + 1];
        }
        
        // Decrement the contactcount by 1[cite: 5]
        addressBook->contactCount--;
        
        printf("Contact deleted successfully!\n");
    } else {
        printf("Deletion cancelled.\n");
    }
}

int validate_phone_no(AddressBook *addressBook, char *phone)
{
    // 1. Check if phone number length is 10
    if (strlen(phone) != 10) {
        return 0;
    }

    // 2. Check if all 10 characters are numeric digits ('0' to '9')
    for (int i = 0; i < 10; i++) {
        if (phone[i] < '0' || phone[i] > '9') {
            return 0;
        }
    }

    // 3. Check if the phone number is unique (not already in the address book)
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0) {
            return 0; // Not unique
        }
    }

    // All conditions true, return 1 for success
    return 1;
}

int validate_email(AddressBook *addressBook, char *email)
{
    int at_count = 0;
    int dot_count = 0;
    char *at_ptr = NULL;
    char *dot_ptr = NULL;

    // 1. Check for uppercase letters and count '@' and '.'
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] >= 'A' && email[i] <= 'Z') {
            return 0; // Upper case should not be present
        }
        if (email[i] == '@') {
            at_count++;
            at_ptr = &email[i];
        }
        else if (email[i] == '.') {
            dot_count++;
            dot_ptr = &email[i];
        }
    }

    // 2. '@' and '.' should be present only once
    if (at_count != 1 || dot_count != 1) {
        return 0;
    }

    // 3. '@' should be present before '.'
    if (at_ptr > dot_ptr) {
        return 0;
    }

    // 4. Before '@', after '.', and between '@' and '.' some chars should be present
    // Check before '@' (email is the start pointer)
    if (at_ptr == email) {
        return 0;
    }
    // Check between '@' and '.'
    if (dot_ptr - at_ptr <= 1) {
        return 0;
    }
    // Check after '.' (must not be the last character)
    if (*(dot_ptr + 1) == '\0') {
        return 0;
    }

    // 5. Check if the email is unique (not already in the address book)
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcmp(addressBook->contacts[i].email, email) == 0) {
            return 0; // Not unique
        }
    }

    // All conditions true, return 1 for success
    return 1;
}
