#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const int MAX_PRODUCTS = 100;

struct Product
{
    string barcode;
    string name;
    int quantity;
    double price;
};

// Global stock array
Product products[MAX_PRODUCTS];
int productCount = 0;

void loadStock()
{
    ifstream fin("stoc.txt");
    if (!fin)
        return;

    fin >> productCount;
    for (int i = 0; i < productCount; ++i)
    {
        fin >> products[i].barcode >> products[i].name >> products[i].quantity >> products[i].price;
    }
    fin.close();
}

void saveStock()
{
    ofstream fout("stoc.txt");
    fout << productCount << '\n';
    for (int i = 0; i < productCount; ++i)
    {
        fout << products[i].barcode << ' ' << products[i].name << ' ' << products[i].quantity << ' ' << fixed << setprecision(2) << products[i].price << '\n';
    }
    fout.close();
}

void viewStock()
{
    for (int i = 0; i < productCount; ++i)
    {
        cout << products[i].barcode << " " << products[i].name << " " << "Quantity: " << products[i].quantity << " " << "Price: " << fixed << setprecision(2) << products[i].price << '\n';
    }
}

void addProduct(const string &barcode, const string &name, int quantity, double price)
{
    for (int i = 0; i < productCount; ++i)
    {
        if (products[i].barcode == barcode)
        {
            cout << "Product already exists.\n";
            return;
        }
    }

    if (productCount < MAX_PRODUCTS)
    {
        products[productCount++] = {barcode, name, quantity, price};
        saveStock();
        cout << "Product added.\n";
    }
    else
    {
        cout << "Stock full. Cannot add more products.\n";
    }
}

void deleteProduct(const string &barcode)
{
    bool found = false;
    for (int i = 0; i < productCount; ++i)
    {
        if (products[i].barcode == barcode)
        {
            for (int j = i; j < productCount - 1; ++j)
            {
                products[j] = products[j + 1];
            }
            productCount--;
            found = true;
            break;
        }
    }
    if (found)
    {
        saveStock();
        cout << "Product deleted.\n";
    }
    else
    {
        cout << "Product not found.\n";
    }
}

void modifyProduct(const string &field, const string &barcode, const string &value)
{
    for (int i = 0; i < productCount; ++i)
    {
        if (products[i].barcode == barcode)
        {
            if (field == "quantity")
            {
                products[i].quantity = stoi(value);
            }
            else if (field == "price")
            {
                products[i].price = stod(value);
            }
            else
            {
                cout << "Invalid field. Use 'quantity' or 'price'.\n";
                return;
            }
            saveStock();
            cout << "Product updated.\n";
            return;
        }
    }
    cout << "Product not found.\n";
}

void viewOrders()
{
    ifstream fin("comenzi.txt");
    if (!fin)
    {
        cout << "No orders found.\n";
        return;
    }

    int day, month, year;
    string barcode;
    while (fin >> day >> month >> year)
    {
        cout << "Order Date: " << day << "/" << month << "/" << year << '\n';
        while (fin >> barcode && barcode != "#")
        {
            cout << "  " << barcode << '\n';
        }
    }

    fin.close();
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: view_stock_products | add_product <b> <n> <q> <p> | delete_product <b> | modify_product <field> <b> <val> | view_orders\n";
        return 1;
    }

    loadStock();

    string cmd = argv[1];

    if (cmd == "view_stock_products")
    {
        viewStock();
    }
    else if (cmd == "add_product" && argc == 6)
    {
        addProduct(argv[2], argv[3], stoi(argv[4]), stod(argv[5]));
    }
    else if (cmd == "delete_product" && argc == 3)
    {
        deleteProduct(argv[2]);
    }
    else if (cmd == "modify_product" && argc == 5)
    {
        modifyProduct(argv[2], argv[3], argv[4]);
    }
    else if (cmd == "view_orders")
    {
        viewOrders();
    }
    else
    {
        cout << "Invalid command or arguments.\n";
    }

    return 0;
}
