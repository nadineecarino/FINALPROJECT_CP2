#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
# include <limits>                      // Added for input validation
using namespace std;

// Temporary arrays to store product details before using a linked list
string productNames[100]; 
int productIds[100];       
double productPrices[100]; 
int productStocks[100]; 
double productSale; 
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
	double totalSales;
	Supplier* supplierList;        // Pointer to a linked list of suppliers
	Product* next;                 // Pointer to the next product in the linked list

	Product(string name, int id, double price, int stock) {
		productName = name;
		productId = id;
		productPrice = price;
		productStock = stock;
		totalSales = 0;
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

SalesNode* salesHead = nullptr;  // Head of sales linked list





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

static void saveInventoryToFile(ProductList& productList) {
	ofstream outFile("inventory.txt");
	if (!outFile) {
		cout << "Error opening file for writing!" << endl;
		return;
	}

	// Table Header
	outFile << left << setw(20) << "Product Name"
		<< setw(10) << "ID"
		<< setw(10) << "Price"
		<< setw(10) << "Stock"
		<< setw(25) << "Supplier Name"
		<< setw(20) << "Phone No."
		<< endl;
	outFile << string(85, '-') << endl; // Horizontal line

	// Writing Product Details
	Product* current = productList.head;
	while (current != nullptr) {
		if (current->supplierList) {
			outFile << left << setw(20) << current->productName
				<< setw(10) << current->productId
				<< setw(10) << fixed << setprecision(2) << current->productPrice
				<< setw(10) << current->productStock
				<< setw(25) << current->supplierList->supplierName
				<< setw(20) << current->supplierList->supplierPhone
				<< endl;
		}
		current = current->next;
	}

	outFile.close();
	cout << "Inventory data saved successfully.\n";
}

// Function to load inventory from a file
static void loadInventoryFromFile(ProductList& productList) {
	ifstream inFile("inventory.txt");
	if (!inFile) {
		cout << "No existing inventory file found. Creating a new one.\n";
		saveInventoryToFile(productList);
		return;
	}

	string name;
	int id = 0, stock = 0;
	double price = 0.0, sales = 0.0;

	while (getline(inFile, name, ',')) { 
		inFile >> id;
		inFile.ignore();
		inFile >> price;
		inFile.ignore();
		inFile >> stock;
		inFile.ignore();
		inFile >> sales;

		Product* newProduct = new Product(name, id, price, stock);
		newProduct->totalSales = sales; 
		newProduct->next = productList.head;
		productList.head = newProduct;
	}

	inFile.close();
	cout << "Inventory loaded successfully.\n";
}




// Function to print total sales with highest and lowest selling products
static void printSalesSummary(Product* head) {
	if (!head) {
		cout << "No products available.\n";
		return;
	}

	double totalSales = 0;
	double highestSale = 0, lowestSale = 1e9;
	string highestProduct = "None", lowestProduct = "None";
	bool hasSales = false;

	cout << "\n==== Sales Summary ====\n";
	Product* current = head;
	while (current) {
		if (current->totalSales > 0) {  // Ensure we count only sold items
			hasSales = true;
			totalSales += current->totalSales;

			if (current->totalSales > highestSale) {
				highestSale = current->totalSales;
				highestProduct = current->productName; 
			}
			if (current->totalSales < lowestSale) {
				lowestSale = current->totalSales;
				lowestProduct = current->productName; 
			}
		}
		current = current->next;
	}

	// Display Sales Summary
	current = head;
	cout << "\nProduct Sales Breakdown:\n";
	cout << "------------------------------------\n";
	while (current) {
		cout << "Product: " << current->productName << " | Total Sales: Php " << current->totalSales << endl;
		current = current->next;
	}

	cout << "------------------------------------\n";
	if (!hasSales) {
		cout << "No sales recorded yet.\n";
	}
	else {
		cout << "Overall Total Sales: Php" << totalSales << endl;
		cout << "Highest Selling Product: " << highestProduct << " (Php" << highestSale << ")\n";
		cout << "Lowest Selling Product: " << lowestProduct << " (Php" << lowestSale << ")\n";
	}
}

// Function to display all products with stock
static void displayAllStock(Product* head) {
	if (!head) {
		cout << "No products in inventory.\n";
		return;
	}

	cout << "\n==== Product Stock List ====\n";
	Product* current = head;
	while (current) {
		cout << "Product: " << current->productName << " | Stock: " << current->productStock << endl;
		current = current->next;
	}
}

// Function to display only out-of-stock products
static void showOutOfStock(Product* head) {
	if (!head) {
		cout << "No products in the inventory.\n";
		return;
	}

	bool anyOutOfStock = false;
	Product* current = head;

	cout << "\n==== Out of Stock Products ====\n";
	while (current) {
		if (current->productStock == 0) {
			cout << current->productName << " is OUT OF STOCK.\n";
			anyOutOfStock = true;
		}
		current = current->next;
	}

	if (!anyOutOfStock) {
		cout << "All products are available.\n";
	}
}

// Function to generate and display sales report
static void generateSalesReport(ProductList& productList) {
	int choice = 0;

	cout << "\n==== Cafe Sales Report ====\n";
	cout << "1. Display Total Sales\n";
	cout << "2. Display All Products with Stock\n";
	cout << "3. Display Out of Stock Products\n";
	cout << "Enter choice: ";

	while (!(cin >> choice) || choice < 1 || choice > 3) {  // Input validation
		cout << "Invalid choice! Please select 1, 2, or 3: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	switch (choice) {
	case 1:
		printSalesSummary(productList.head);
		break;
	case 2:
		displayAllStock(productList.head);
		break;
	case 3:
		showOutOfStock(productList.head);
		break;
	}
}









// Function to record sales  
static void trackSales(ProductList& productList) {
	int productId, quantity;
	double totalAmount;
	cout << "Record a Sale (Reduce Stock)\n";
	

	cout << "Enter Product ID: ";
	cin >> productId;

	Product* current = productList.head;
	while (current) {
		if (current->productId == productId) 
		{
			  // Track sales (Reduce stock)
				cout << "Enter quantity sold: ";
				cin >> quantity;

				if (quantity > current->productStock) {
					cout << "Error: Not enough stock available\n";
					return;
				}
				current->productStock -= quantity;
				totalAmount = quantity * current->productPrice;
				cout << "Sale recorded. Remaining stock: " << current->productStock << endl;

				
				// Add sale record to linked list
				SalesNode* newSale = new SalesNode(totalAmount);
				newSale->next = salesHead;
				salesHead = newSale;
		
			saveInventoryToFile(productList); // Save updated inventory
			return;
		}
		current = current->next;
	}
	cout << "Product not found!\n";
	return;
}

//Function to update product information
static void updateProduct(ProductList& productList) {
	int id = 0;
	cout << "Enter Product ID to update: ";
	cin >> id; 
	Product* current = productList.head;
	while (current != nullptr) {
		
		if (current->productId == id) {
			cout << "Updating product: " << current->productName << endl;
			cout << "Enter new name: ";
			string newName;
			cin.ignore();
			getline(cin, newName);
			if (!newName.empty()) {
				current->productName = newName;
			}

			cout << "Enter new price: ";
			double newPrice;
			cin >> newPrice;
			if (newPrice >= 0) {
				current->productPrice = newPrice;
			}

			cout << "Enter new stock: ";
			int newStock;
			cin >> newStock;
			if (newStock >= 0) {
				current->productStock = newStock;
			}

			// Save updated inventory to file
			saveInventoryToFile(productList);
		}
		current = current->next;
	}
	cout << "Product ID not found!\n";
}

// Function to check if a product ID already exists
static bool isProductIdUnique(ProductList& productList, int id) {
	Product* current = productList.head;
	while (current != nullptr) {
		if (current->productId == id) {
			return false; // ID already exists
		}
		current = current->next;
	}
	return true; // ID is unique
}
// Function to add product
static void addProduct(ProductList& productList)
{
	ofstream inFile("inventory.txt", ios::app);
	if (!inFile) {
		cout << "No existing inventory file found.\n";
		return;
	}

	string supplierName;
	int supplierNumber;

	Product* newProduct = new Product("", 0, 0.0, 0);  

	cout << "ADD NEW PRODUCT" << endl;
	cout << "Product Name: ";
	cin >> ws;
	getline(cin, newProduct->productName);
	do {
		cout << "ID: ";
		cin >> newProduct->productId;
		if (!isProductIdUnique(productList, newProduct->productId)) {
			cout << "Error: Product ID already exists! Please enter a unique ID.\n";
		}
	} while (!isProductIdUnique(productList, newProduct->productId));
	
	cout << "Price: ";
	cin >> newProduct->productPrice;
	cout << "Stock: ";
	cin >> newProduct->productStock;
	cout << "Supplier Name: ";
	cin.ignore();
	getline(cin, supplierName);
	cout << "\nPhone No.: ";
	cin >> supplierNumber;

	

	newProduct->addSupplier(supplierName, supplierNumber);
	newProduct->next = productList.head;                        // Add new product to the linked list
	productList.head = newProduct;
	saveInventoryToFile(productList);

	cout << "Product added successfully!\n";


	inFile << left << setw(20) << newProduct->productName         // Save to file with proper table alignment
		<< setw(10) << newProduct->productId
		<< setw(10) << fixed << setprecision(2) << newProduct->productPrice
		<< setw(10) << newProduct->productStock
		<< setw(20) << supplierName
		<< setw(15) << supplierNumber
		<< endl;

	inFile.close();

	saveInventoryToFile(productList);
	cout << "Product added successfully!\n";

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

			saveInventoryToFile(productList); 
            return; 
        }
        previous = current; 
        current = current->next; 

		 
    }
    cout << "Product ID not found!\n";
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

//Function to display the products
static void displayProducts(ProductList& productList) {
	cout << "\nINVENTORY\n";
	cout << string(85, '-') << endl; // Horizontal line
	cout << left << setw(20) << "Product Name"
		<< setw(10) << "ID"
		<< setw(10) << "Price"
		<< setw(10) << "Stock"
		<< setw(25) << "Supplier Name"
		<< setw(20) << "Phone No."
		<< endl;
	cout << string(85, '-') << endl; // Horizontal line

	Product* current = productList.head;
	while (current != nullptr) {
		if (current->supplierList) {
			cout << left << setw(20) << current->productName
				<< setw(10) << current->productId
				<< setw(10) << fixed << setprecision(2) << current->productPrice
				<< setw(10) << current->productStock
				<< setw(25) << current->supplierList->supplierName
				<< setw(20) << current->supplierList->supplierPhone
				<< endl;
		}
		current = current->next;
	}

	cout << string(85, '-') << endl; // Horizontal line
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
		else if (temp->productName == "Syrup") {
			temp->addSupplier("Sweet Syrup Suppliers", 912345566);
		}
		temp = temp->next;
	}


	//Main Menu System
	saveInventoryToFile(productList);
	loadInventoryFromFile(productList); 


	char choice;
	do {
		cout << "Welcome to Bubble's  Cafe  Inventory\n";
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



		switch (toupper(choice))
		{
		case 'A':                                                                            //Call sales report function
			generateSalesReport(productList);
			break;
		case 'B':                                                                            // Track sales and update inventory                                              
		{
			cout << "\n\n===========================================\n";
			cout << "Track Sales & Update Inventory:\n\n";
			trackSales(productList);                                                        
			cout << "\n===========================================\n\n";
		}
			break;
		case 'C':
		{
			cout << "\n\n===========================================\n";
			cout << "Update Product Information\n";
			updateProduct(productList);
			cout << "\n===========================================\n\n";
		}
			break;
		case 'D':
		{
			cout << "\n\n===========================================\n";
			addProduct(productList);
			cout << "\n\n===========================================\n";
		}
			break;
		case 'E':                                                                         // Call delete function
		{
			int productId;
			cout << "\n\n===========================================\n";
			cout << "Enter the Product ID to delete: ";
			cin >> productId;
			cout << "\n===========================================\n\n";

			deleteProduct(productList, productId);
		}
			break;
		case 'F':                                                                          // Call search product function
		{
			string productName;
			cout << "\n\n===========================================\n";
			cout << "Search Product: ";
			getline(cin, productName);
			cout << "\n";
			Product* foundProduct = searchProduct(productList.head, productName);
			cout << "\n===========================================\n\n";
		}
		break;
		case 'G':
		{
			cout << "\n\n===========================================\n";
			cout << "Displaying All Products:\n\n";
			displayProducts(productList);
			cout << "\n===========================================\n\n";
		}
			break;
		case 'H':
		{
			saveInventoryToFile(productList);
			loadInventoryFromFile(productList);
		}
			break;
		case 'I':
		{
			cout << "\n\n===========================================\n";
			cout << "Exiting Bubble's  Cafe Inventory\n";
			cout << "\n\n===========================================\n";
		}
			break;
		default:
			cout << "Invalid. Please Enter a Valid Choice\n";
			break;

		}
	} while (toupper(choice != 'I'));

	
	return 0;
}
