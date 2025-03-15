#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

// Temporary arrays to store product details before using a linked list
string productNames[100]; 
int productIds[100];       
double productPrices[100]; 
int productStocks[100];    
int productCount = 0; // Counter to track the number of products added to temporary storage.


// Struct for Supplier Information
struct Supplier
{   string supplierName;
	int supplierPhone;
	Supplier* next; // Pointer to the next supplier in the linked list

	Supplier(string name, int phone) //initialize a supplier with a name and phone number
	{   supplierName = name;
		supplierPhone = phone;
		next = nullptr;
	}
};


// Struct for Product Information
struct Product {
	string productName;
	int productId;
	double productPrice;
	int productStock;
	Supplier* supplierList;        // Pointer to a linked list of suppliers
	Product* next;                 // Pointer to the next product in the linked list

	Product(string name, int id, double price, int stock) {
		productName = name;
		productId = id;
		productPrice = price;
		productStock = stock;
		supplierList = nullptr; // No suppliers linked initially.
		next = nullptr;
	}
	// Function to add a supplier to the product's supplier list
	void addSupplier(string name, int phone) {
		Supplier* newSupplier = new Supplier(name, phone); // Create a new supplier node.
		newSupplier->next = supplierList; // Insert the new supplier at the beginning.
		supplierList = newSupplier;       // Update the supplier list pointer.
	}
};

// Struct for Calculating 
struct SalesNode 
{   double amount;
	SalesNode* next;

	SalesNode(double amt) 
	{   amount = amt;
		next = nullptr;
	}
};




// Linked List for products
class ProductList
{public:
	Product* head;          // Pointer to the first product in the linked list.
	ProductList()           // Constructor to initialize an empty product list
	{
		head = nullptr;     // Initially, no products are linked.
	}
};








// Function to save inventory details to file
static void saveInventoryToFile(ProductList& productList)
{
	ofstream outFile("inventory.txt");
	if (!outFile)
	{
		cout << "Error opening file for writing!" << endl;
		return;
	}

	// Write table header
	outFile << "Product Name     ID     Price    Stock\n";
	outFile << "--------------------------------------\n";

	Product* currentProduct = productList.head;
	while (currentProduct != nullptr)
	{
		outFile << left << setw(15) << currentProduct->productName
			<< setw(6) << currentProduct->productId
			<< setw(9) << fixed << setprecision(2) << currentProduct->productPrice
			<< setw(6) << currentProduct->productStock << "\n";

		currentProduct = currentProduct->next;
	}

	outFile.close();
	cout << "Inventory data saved successfully.\n";
}


// Function to load inventory data from a file
static void loadInventoryFromFile(ProductList& productList)
{   ifstream inFile("inventory.txt");
	if (!inFile) 
	{   cout << "No existing inventory file found.\n";
		return;
	}

	string name;
	int id, stock;
	double price;
	while (inFile >> name >> id >> price >> stock) 
	{   Product* newProduct = new Product(name, id, price, stock);
		newProduct->next = productList.head;
		productList.head = newProduct;
	}
	inFile.close();
	cout << "Inventory data loaded successfully.\n";
}

// Function to load inventory from a file
static void loadInventory(ProductList& productList) {
	ifstream inFile("inventory.txt");
	if (!inFile) {
		cout << "No existing inventory file found. Creating a new one.\n";
		saveInventoryToFile(productList);
		return;
	}

	string name;
	int id, stock;
	double price;
	char comma;

	while (inFile >> ws, getline(inFile, name, ',')) {
		if (!(inFile >> id >> comma >> price >> comma >> stock)) {
			cout << "Error: Invalid file format. Skipping line.\n";
			inFile.clear();
			inFile.ignore(1000, '\n');
			continue;
		}

		Product* newProduct = new Product(name, id, price, stock);
		newProduct->next = productList.head;
		productList.head = newProduct;
	}

	inFile.close();
	cout << "Inventory loaded successfully.\n";
}

//Function to delete a product
static void deleteProduct(ProductList &productList, int id) {
    Product* current = productList.head;
    Product* previous = nullptr;

    while (current != nullptr) {
		if (current->productId == id) {
		    if (previous == nullptr) {
			  productList.head = current->next; 
            } 
		    else {
			   previous->next = current->next; 
            }
            delete current; 
            cout << "Product deleted successfully!\n";
            return; 
        }
        previous = current; 
        current = current->next; 
    }
    cout << "Product ID not found!\n";
}



// Function to calculate and display total sales
static void displayTotalSales(SalesNode* head) {
	if (!head) {
		cout << "No sales records available." << endl;
		return;
	}

	double totalSales = 0, min = head->amount, max = head->amount;
	int count = 0;

	SalesNode* current = head;
	while (current) {
		totalSales += current->amount;   // Add sale amount to total
		if (current->amount < min) min = current->amount;  // Update min sale
		if (current->amount > max) max = current->amount;  // Update max sale
		count++;  // Increase sale count
		current = current->next;  // Move to next sale
	}

	cout << "\nTotal Sales: " << totalSales << endl;
	cout << "Lowest Sale: " << min << endl;
	cout << "Highest Sale: " << max << endl;
}



// Function to search for a product by name in the inventory
static Product* searchProduct(Product* head, const string& name) {
	Product* current = head;
	while (current) {
		if (current->productName == name) {
			cout << "Product Found: " << current->productName
				<< " | ID: " << current->productId
				<< " | Price: " << current->productPrice
				<< " | Stock: " << current->productStock << endl;
			return current; // Return the found product
		}
		current = current->next;
	}
	cout << "Product not found in inventory.\n";
	return nullptr; // Product not found
}

// Function for displaying remaining stock for each product	
static void displayAllStock(Product * head) {
		if (!head) {
			cout << "No products in inventory." << endl;
			return;
		}
		cout << "\nRemaining Stock:\n";
		Product* current = head;
		while (current) {
			cout << current->productName << " | Stock: " << current->productStock << endl;
			current = current->next;
		}
}



// Function to check and display out-of-stock products
static void showOutOfStock(Product* head) {
	if (!head) {
		cout << "No products in the inventory." << endl;
		return;
	}

	bool anyOutOfStock = false;
	Product* current = head;

	while (current) {
		if (current->productStock == 0) {
			cout << current->productName << " is OUT OF STOCK." << endl;
			anyOutOfStock = true;
		}
		current = current->next;
	}
	if (!anyOutOfStock) {
		cout << "All products are available." << endl;
	}
}



// Function to generate a total sales report
static void generateSalesReport(SalesNode* salesHead, Product* productHead) {
	cout << "\n===== SALES REPORT =====" << endl;
	displayTotalSales(salesHead);                                                // Display total sales
	cout << "----------------------" << endl;

	cout << "\nRemaining Stock:" << endl; 
	displayAllStock(productHead);                                                // Display remaining stock
	cout << "----------------------" << endl;

	cout << "\nOut of Stock Products:" << endl;                                   // Display out-of-stock products
	showOutOfStock(productHead);
	cout << "=======================" << endl;
}

int main()
{
	// Storing products temporarily in arrays before transferring to linked list
	productNames[0] = "Coffee";
	productIds[0] = 101;
	productPrices[0] = 120;
	productStocks[0] = 10;

	productNames[1] = "Milk";
	productIds[1] = 102;
	productPrices[1] = 89;
	productStocks[1] = 20;

	productNames[2] = "Whipped Cream";
	productIds[2] = 103;
	productPrices[2] = 100;
	productStocks[2] = 20;

	productNames[3] = "Chocolate";
	productIds[3] = 104;
	productPrices[3] = 100;
	productStocks[3] = 20;

	productNames[4] = "Ice";
	productIds[4] = 105;
	productPrices[4] = 59;
	productStocks[4] = 20;

	productNames[5] = "Syrup";
	productIds[5] = 106;
	productPrices[5] = 100;
	productStocks[5] = 20;
	productCount = 6;                                                                                        // Indicates the number of products stored temporarily.


	ProductList productList;                                                                                  // Create an instance of ProductList to store products.
	SalesNode* salesHead = nullptr;                                                                           // Initialize sales linked list

	// Adding products from arrays into the linked list
	Product* last = nullptr;                                                                                      // Pointer to track the last product added.
	for (int i = 0; i < productCount; i++) {
		Product* newProduct = new Product(productNames[i], productIds[i], productPrices[i], productStocks[i]); // Create a new product node.
		newProduct->next = productList.head;                                                                      // Insert the product at the beginning of the list.
		productList.head = newProduct;                                                                            // Update the head pointer.
		if (last == nullptr) {
			last = newProduct;                                                                                    // Assign the first product as the last.
		}
	}

	// Adding Supplier Information to Every Products
	Product* temp = productList.head;
	while (temp != nullptr) {
		if (temp->productName == "Coffee") {
			temp->addSupplier("Coffee Supplier Co.", 123456789);
		}
		else if (temp->productName == "Milk") {
			temp->addSupplier("Dairy Fresh Ltd.", 987654321);
		}
		else if (temp->productName == "Whipped Cream") {
			temp->addSupplier("WhipIt Industries", 112233445);
		}
		else if (temp->productName == "Chocolate") {
			temp->addSupplier("Choco Delights", 556677889);
		}
		else if (temp->productName == "Ice") {
			temp->addSupplier("Cool Ice Suppliers", 998877665);
		}
		temp = temp->next;
	}


	//Main Menu System
	saveInventoryToFile(productList);
	loadInventoryFromFile(productList);


	char choice;
	do {
		cout << "Welcome to Bubble's  Cafe Inventory\n";
		cout << "Menu\n\n";
		cout << "A.Sales Report\n"
			<< "B.Track sales and Update Inventory\n"
			<< "C.Update Product Information\n"
			<< "D.Add New Product\n"
			<< "E.Delete a Product\n"
			<< "F.Search a Product\n"
			<< "G.Display Product\n"
			<< "H.Save inventory to a file and load inventory from a file\n"
			<< "I. Exit\n";

		cout << "Inventory Choice: ";
		cin >> choice;
		cin.ignore();



		switch (choice)
		{
		case 'A':                                                                            //Call sales report function
			generateSalesReport(salesHead, productList.head);
			break;
		case 'B':                                                                           // Call search product function

			break;
		case 'C':

			break;
		case 'D':

			break;
		case 'E':
			int productId;
			cout << "Enter the Product ID to delete: ";
			cin >> productId;

			deleteProduct(productList, productId);                                          // Call delete function
			saveInventoryToFile(productList);                                               // Save updated inventory
			break;
		case 'F':                                                                          // Call search product function
		{
			string productName;
			cout << "\n\n===========================================\n";
			cout << "Search Product: ";
			getline(cin, productName);
			cout << "\n";
			Product* foundProduct = searchProduct(productList.head, productName);
		}
		break;
		case 'G':

			break;
		case 'H':

			break;
		case 'I':

			break;
		default:
			cout << "Invalid. Please Enter a Valid Choice\n";
			break;

		}
	} while (choice != 'I');


	return 0;

}