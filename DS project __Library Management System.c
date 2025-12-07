
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book {
    int id;
    char title[100];
    char author[100];
    char genre[50];
    int numCopies;
    int borrowedCount;
    struct Book* next;
} Book;

typedef struct User {
    int id;
    char name[100];
    struct User* next;
} User;

typedef struct Borrowed {
    int userId;
    int bookId;
    struct Borrowed* next;
} Borrowed;

Book* bookHead = NULL;
User* userHead = NULL;
Borrowed* borrowStack = NULL;

void addBook(int id, char title[], char author[], char genre[], int numCopies);
void viewBooks();
void searchBook(int id);
void deleteBook(int id);
void registerUser(int id, char name[]);
void viewUsers();
void searchUser(int id);
void borrowBook(int userId, int bookId);
void returnBook(int bookId);
void pushBorrowed(int userId, int bookId);
void popBorrowed();
void viewBorrowedStack();
void managementMenu();
void userMenu();
void aboutUs();
void loginOrRegisterUser(int* userId);

void addBook(int id, char title[], char author[], char genre[], int numCopies) {
    Book* newBook = (Book*)malloc(sizeof(Book));
    newBook->id = id;
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    strcpy(newBook->genre, genre);
    newBook->numCopies = numCopies;
    newBook->borrowedCount = 0;
    newBook->next = NULL;

    if (bookHead == NULL) {
        bookHead = newBook;
    } else {
        Book* temp = bookHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newBook;
    }
}


void viewBooks() {
    Book* temp = bookHead;
    printf("\n--- Book List ---\n");
    while (temp != NULL) {
        printf("ID: %d, Title: %s,   Author: %s,   Genre: %s,   Available Copies: %d\n",
               temp->id, temp->title, temp->author, temp->genre, temp->numCopies - temp->borrowedCount);
        temp = temp->next;
    }
}


void searchBookByCriteria() {
    int choice;
    char searchTerm[100];

    printf("\n--- Search Book ---\n");
    printf("Choose search criteria:\n");
    printf("1. By Name\n2. By Author\n3. By Genre\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar();


    printf("Enter search term: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;

    Book* temp = bookHead;
    int found = 0;

    while (temp != NULL) {
        switch (choice) {
            case 1:

                if (strstr(temp->title, searchTerm) != NULL) {
                    printf("Found: ID: %d, Title: %s,   Author: %s,   Genre: %s,   Available Copies: %d\n",
                           temp->id, temp->title, temp->author, temp->genre, temp->numCopies - temp->borrowedCount);
                    found = 1;
                }
                break;
            case 2:

                if (strstr(temp->author, searchTerm) != NULL) {
                    printf("Found: ID: %d, Title: %s,   Author: %s,   Genre: %s,   Available Copies: %d\n",
                           temp->id, temp->title, temp->author, temp->genre, temp->numCopies - temp->borrowedCount);
                    found = 1;
                }
                break;
            case 3:

                if (strstr(temp->genre, searchTerm) != NULL) {
                    printf("Found: ID: %d, Title: %s,   Author: %s,   Genre: %s,   Available Copies: %d\n",
                           temp->id, temp->title, temp->author, temp->genre, temp->numCopies - temp->borrowedCount);
                    found = 1;
                }
                break;
            default:
                printf("Invalid choice.\n");
                return;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("No books found with the given criteria.\n");
    }
}

void deleteBook(int id) {
    Book* temp = bookHead;
    Book* prev = NULL;
    while (temp != NULL) {
        if (temp->id == id) {
            if (prev == NULL) {
                bookHead = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            printf("Book deleted successfully!\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Book not found.\n");
}


void registerUser(int id, char name[]) {
    User* newUser = (User*)malloc(sizeof(User));
    newUser->id = id;
    strcpy(newUser->name, name);
    newUser->next = userHead;
    userHead = newUser;

}

void viewUsers() {
    User* temp = userHead;
    printf("\n--- User List ---\n");
    while (temp != NULL) {
        printf("ID: %d, Name: %s\n", temp->id, temp->name);
        temp = temp->next;
    }
}

void searchUser(int id) {
    User* temp = userHead;
    while (temp != NULL) {
        if (temp->id == id) {
            printf("User Found: ID: %d, Name: %s\n", temp->id, temp->name);
            return;
        }
        temp = temp->next;
    }
    printf("User not found.\n");
}

void borrowBook(int userId, int bookId) {
    User* user = userHead;
    while (user != NULL && user->id != userId) {
        user = user->next;
    }
    if (user == NULL) {
        printf("User not found.\n");
        return;
    }

    Book* book = bookHead;
    while (book != NULL && book->id != bookId) {
        book = book->next;
    }
    if (book == NULL) {
        printf("Book not found.\n");
        return;
    }

    if (book->borrowedCount >= book->numCopies) {
        printf("No copies available for borrowing.\n");
    } else {
        book->borrowedCount++;
        pushBorrowed(userId, bookId);
        printf("Book borrowed successfully!\n");
    }
}

void returnBook(int bookId) {
    Book* book = bookHead;
    while (book != NULL && book->id != bookId) {
        book = book->next;
    }
    if (book == NULL) {
        printf("Book not found.\n");
        return;
    }

    if (book->borrowedCount <= 0) {
        printf("No borrowed copies to return.\n");
    } else {
        book->borrowedCount--;
        popBorrowed();
        printf("Book returned successfully!\n");
    }
}

void pushBorrowed(int userId, int bookId) {
    Borrowed* newEntry = (Borrowed*)malloc(sizeof(Borrowed));
    newEntry->userId = userId;
    newEntry->bookId = bookId;
    newEntry->next = borrowStack;
    borrowStack = newEntry;
}

void popBorrowed() {
    if (borrowStack == NULL) {
        printf("Borrowed stack is empty.\n");
        return;
    }
    Borrowed* temp = borrowStack;
    borrowStack = borrowStack->next;
    free(temp);
}

void viewBorrowedStack() {
    if (borrowStack == NULL) {
        printf("No borrowed books to display.\n");
        return;
    }
    printf("\n--- Borrowed Book List ---\n");
    Borrowed* temp = borrowStack;
    while (temp != NULL) {
        printf("User ID: %d, Book ID: %d\n", temp->userId, temp->bookId);
        temp = temp->next;
    }
}


void managementMenu() {
        char accessKey[10];
    printf("Enter Management Access Key: ");
    scanf("%s", accessKey);

    if (strcmp(accessKey, "DIU-242") != 0) {
        printf("Invalid Access Key. Access Denied.\n");
        return;
    }

    int choice, id, numCopies;
    char title[100], author[100], genre[50];
    while (1) {
        printf("\n--- Management Menu ---\n");
        printf("1. Add Book\n2. View Books\n3. Search Book\n4. Delete Book\n5. View Users\n6. Search User\n7. View Borrowed Books\n8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Book ID, Title, Author, Genre, and Number of Copies: ");
                scanf("%d", &id);
                getchar();
                fgets(title, 100, stdin);
                title[strcspn(title, "\n")] = 0;
                fgets(author, 100, stdin);
                author[strcspn(author, "\n")] = 0;
                fgets(genre, 50, stdin);
                genre[strcspn(genre, "\n")] = 0;
                scanf("%d", &numCopies);
                addBook(id, title, author, genre, numCopies);
                break;

            case 2:
                viewBooks();
                break;

            case 3:
                searchBookByCriteria();
                break;

            case 4:
                printf("Enter Book ID: ");
                scanf("%d", &id);
                deleteBook(id);
                break;

            case 5:
                viewUsers();
                break;

            case 6:
                printf("Enter User ID: ");
                scanf("%d", &id);
                searchUser(id);
                break;

            case 7:
                viewBorrowedStack();
                break;

            case 8:
                return;

            default:
                printf("Invalid choice!\n");
        }
    }
}

void loginOrRegisterUser(int* userId) {
    int choice;
    printf("\n--- Login / Register ---\n");
    printf("1. Login\n2. Register\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            {
                printf("Enter User ID: ");
                scanf("%d", userId);
                User* user = userHead;
                while (user != NULL) {
                    if (user->id == *userId) {
                        printf("Login successful! Welcome, %s.\n", user->name);
                        return;
                    }
                    user = user->next;
                }
                printf("User not found. Please register first.\n");
                loginOrRegisterUser(userId);
                break;
            }
        case 2:
            {
                printf("Enter User ID: ");
                scanf("%d", userId);
                getchar();
                char name[100];
                printf("Enter Name: ");
                fgets(name, 100, stdin);
                name[strcspn(name, "\n")] = 0;

                registerUser(*userId, name);
                break;
            }
        default:
            printf("Invalid choice!\n");
            loginOrRegisterUser(userId);
            break;
    }
}


void userMenu() {
    int choice, userId, bookId;

    loginOrRegisterUser(&userId);

    while (1) {
        printf("\n--- Student Menu ---\n");
        printf("1. View Books\n2. Search Book\n3. Borrow Book\n4. Return Book\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewBooks();
                break;

            case 2:
                searchBookByCriteria();
                break;

            case 3:
                printf("Enter Book ID: ");
                scanf("%d", &bookId);
                borrowBook(userId, bookId);
                break;

            case 4:
                printf("Enter Book ID: ");
                scanf("%d", &bookId);
                returnBook(bookId);
                break;

            case 5:
                return;

            default:
                printf("Invalid choice!\n");
        }
    }
}

void aboutUs()
{
    printf("\tPartha Majumder\n");
    printf("\tProject Manager\n");
    printf("\t242-15-193\n");
    printf("\t================\n\n\n");
    printf("\tAhmed Shoyeb Sani\n");
    printf("\tSystem Designer\n");
    printf("\t242-15-163\n");
    printf("\t===============\n\n\n");
    printf("\tA.B.M Saiem\n");
    printf("\tDebugger\n");
    printf("\t242-15-094\n");
    printf("\t========================\n\n\n");
    printf("\tIshtiak Ahmed Emon\n");
    printf("\tCode Writer\n");
    printf("\t242-15-179\n");
    printf("\t==================\n\n\n");
    printf("\tMahidul Islam Mahi\n");
    printf("\tCode Writer\n");
    printf("\t242-15-161\n");
    printf("\t==================\n\n\n");

}

int main() {

    addBook(1, "The Great Gatsby", "F. Scott Fitzgerald", "Fiction", 5);
    addBook(2, "To Kill a Mockingbird", "Harper Lee", "Fiction", 3);
    addBook(3, "1984", "George Orwell", "Fiction", 4);
    addBook(4, "The Catcher in the Rye", "J.D. Salinger", "Fiction", 3);
    addBook(5, "Brave New World", "Aldous Huxley", "Fiction", 2);
    addBook(6, "Moby-Dick", "Herman Melville", "Adventure", 2);
    addBook(7, "War and Peace", "Leo Tolstoy", "Historical Fiction", 6);
    addBook(8, "Pride and Prejudice", "Jane Austen", "Romance", 7);
    addBook(9, "The Lord of the Rings", "J.R.R. Tolkien", "Fantasy", 5);
    addBook(10, "The Odyssey", "Homer", "Epic Poetry", 2);

    registerUser(101, "Partha Majumder");
    registerUser(102, "Ahmed Shoyeb Sani");
    registerUser(103, "A.B.M Saiem");
    registerUser(104, "Mahidul Islam Mahi");
    registerUser(105, "Istiak Ahmed Emon");
    int accessType;
    printf("\t\t\t\t  ==============================\n");
    printf("\t\t\t\t  ||     DIU SMART LIBRARY    ||\n");
    printf("\t\t\t\t  ==============================\n\n");

    while (1) {
            printf("\t  -----------------------       --------------------       --------------       ----------\n");
            printf("\t1.|  Management Access  |     2.|  Student access  |     3.|  About Us  |     4.|  Exit  |\n");
            printf("\t  -----------------------       --------------------       --------------       ----------\n\n");
        printf("Enter your choice : ");
        scanf("%d", &accessType);

        switch (accessType) {
            case 1:
                managementMenu();
                break;

            case 2:
                userMenu();
                break;

            case 3:
                aboutUs();
                break;

            case 4:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid access type!\n");
        }
    }
    return 0;
}
