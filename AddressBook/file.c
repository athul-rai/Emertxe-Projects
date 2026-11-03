#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) {
    FILE *fptr = fopen("contacts.csv", "w");
    if (fptr == NULL) {
        printf("Error: Could not open contacts.csv for writing!\n");
        return;
    }
    
    fprintf(fptr, "%d\n", addressBook->contactCount);
    
    for (int i = 0; i < addressBook->contactCount; i++) {
        fprintf(fptr, "%s,%s,%s\n", 
                addressBook->contacts[i].name, 
                addressBook->contacts[i].phone, 
                addressBook->contacts[i].email);
    }
    
    fclose(fptr);
}

void loadContactsFromFile(AddressBook *addressBook) {
    FILE *fptr = fopen("contacts.csv", "r");
    if (fptr == NULL) {
        addressBook->contactCount = 0;
        return;
    }
    
    if (fscanf(fptr, "%d", &addressBook->contactCount) != 1) {
        addressBook->contactCount = 0;
        fclose(fptr);
        return;
    }
    
    char ch = fgetc(fptr);
    if (ch != '\n' && ch != EOF) {
        ungetc(ch, fptr);
    }
    
    for (int i = 0; i < addressBook->contactCount; i++) {
        fscanf(fptr, "%[^,],%[^,],%[^\n]\n", 
               addressBook->contacts[i].name, 
               addressBook->contacts[i].phone, 
               addressBook->contacts[i].email);
    }
    
    fclose(fptr);
}