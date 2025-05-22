#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_CART_ITEMS = 100;
const int MAX_PRODUCTS = 100;

struct CartItem
{
    string barcode;
    int quantity;
};

struct Product
{
    string barcode;
    string name;
    int quantity;
    double price;
};

CartItem cart[MAX_CART_ITEMS];
int cartCount = 0;

// ---------- Cart Functions ----------

void loadCart()
{
    ifstream fin("cos_cumparaturi.txt");
    cartCount = 0;
    while (fin >> cart[cartCount].barcode >> cart[cartCount].quantity)
    {
        cartCount++;
    }
    fin.close();
}

void saveCart()
{
    ofstream fout("cos_cumparaturi.txt");
    for (int i = 0; i < cartCount; ++i)
    {
        fout << cart[i].barcode << ' ' << cart[i].quantity << '\n';
    }
    fout.close();
}

void viewCart()
{
    if (cartCount == 0)
    {
        cout << "Shopping cart is empty.\n";
        return;
    }
    cout << "SHOPPING CART:\n";
    for (int i = 0; i < cartCount; ++i)
    {
        cout << cart[i].barcode << " - Quantity: " << cart[i].quantity << '\n';
    }
}

void addToCart(const string &barcode, int quantity)
{
    for (int i = 0; i < cartCount; ++i)
    {
        if (cart[i].barcode == barcode)
        {
            cart[i].quantity += quantity;
            saveCart();
            cout << "Product quantity updated in cart.\n";
            return;
        }
    }

    if (cartCount < MAX_CART_ITEMS)
    {
        cart[cartCount++] = {barcode, quantity};
        saveCart();
        cout << "Product added to cart.\n";
    }
    else
    {
        cout << "Cart is full.\n";
    }
}

void modifyCartProduct(const string &barcode, int newQuantity)
{
    for (int i = 0; i < cartCount; ++i)
    {
        if (cart[i].barcode == barcode)
        {
            cart[i].quantity = newQuantity;
            saveCart();
            cout << "Cart product updated.\n";
            return;
        }
    }
    cout << "Product not found in cart.\n";
}

void deleteCartProduct(const string &barcode)
{
    for (int i = 0; i < cartCount; ++i)
    {
        if (cart[i].barcode == barcode)
        {
            for (int j = i; j < cartCount - 1; ++j)
                cart[j] = cart[j + 1];
            cartCount--;
            saveCart();
            cout << "Product removed from cart.\n";
            return;
        }
    }
    cout << "Product not found in cart.\n";
}

// ---------- Purchase Function ----------

void purchase()
{
    Product products[MAX_PRODUCTS];
    int productCount = 0;

    // Load stock
    ifstream fin("stoc.txt");
    if (!fin)
    {
        cout << "Stock file not found.\n";
        return;
    }

    fin >> productCount;
    for (int i = 0; i < productCount; ++i)
    {
        fin >> products[i].barcode >> products[i].name >> products[i].quantity >> products[i].price;
    }
    fin.close();

    // Check if all products in cart are available
    for (int i = 0; i < cartCount; ++i)
    {
        bool found = false;
        for (int j = 0; j < productCount; ++j)
        {
            if (products[j].barcode == cart[i].barcode)
            {
                found = true;
                if (products[j].quantity < cart[i].quantity)
                {
                    cout << "Insufficient quantity for " << cart[i].barcode << '\n';
                    return;
                }
                break;
            }
        }
        if (!found)
        {
            cout << "Product not found in stock: " << cart[i].barcode << '\n';
            return;
        }
    }

    // Update stock
    for (int i = 0; i < cartCount; ++i)
    {
        for (int j = 0; j < productCount; ++j)
        {
            if (products[j].barcode == cart[i].barcode)
            {
                products[j].quantity -= cart[i].quantity;
            }
        }
    }

    // Save updated stock
    ofstream foutStock("stoc.txt");
    foutStock << productCount << '\n';
    for (int i = 0; i < productCount; ++i)
    {
        foutStock << products[i].barcode << ' ' << products[i].name << ' '
                  << products[i].quantity << ' ' << products[i].price << '\n';
    }
    foutStock.close();

    // Append to orders
    ofstream foutOrder("comenzi.txt", ios::app);
    time_t now = time(0);
    tm *local = localtime(&now);
    foutOrder << local->tm_mday << ' ' << local->tm_mon + 1 << ' ' << local->tm_year + 1900 << '\n';
    for (int i = 0; i < cartCount; ++i)
    {
        foutOrder << cart[i].barcode << ' ';
    }
    foutOrder << "#\n";
    foutOrder.close();

    // Clear cart
    cartCount = 0;
    saveCart();

    cout << "Purchase completed successfully.\n";
}

// ---------- Main ----------

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: view_cart | add_product <b> <q> | modify_product <b> <q> | delete_product <b> | purchase\n";
        return 1;
    }

    string cmd = argv[1];
    loadCart();

    if (cmd == "view_cart")
    {
        viewCart();
    }
    else if (cmd == "add_product" && argc == 4)
    {
        addToCart(argv[2], stoi(argv[3]));
    }
    else if (cmd == "modify_product" && argc == 4)
    {
        modifyCartProduct(argv[2], stoi(argv[3]));
    }
    else if (cmd == "delete_product" && argc == 3)
    {
        deleteCartProduct(argv[2]);
    }
    else if (cmd == "purchase")
    {
        purchase();
    }
    else
    {
        cout << "Invalid command or arguments.\n";
    }

    return 0;
}
