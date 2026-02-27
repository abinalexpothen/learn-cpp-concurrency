#include <thread>
#include <string>
#include <iostream>
#include <tracy/Tracy.hpp>

void open_doc(std::string path)
{
    ZoneScoped;
    // simulate opening document and displaying gui
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate time taken to open document
}

bool done_editing()
{
    ZoneScoped;
    // Simulate checking if the user is done editing
    return false; // For demonstration purposes, we will never be done
}

void edit_document(const std::string &path)
{
    ZoneScoped;
    open_doc(path);

    while (!done_editing())
    {
        ZoneScoped;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate time taken for user to edit
        std::cout << "Editing document: " << path << std::endl;
        std::string user_input;
        // Simulate editing the document
        std::cin >> user_input;

        if (user_input == "open_doc")
        {
            ZoneScoped;
            std::cout << "Enter new document path: ";
            std::string new_file_name;
            std::cin >> new_file_name;
            std::thread t(edit_document, new_file_name);
            t.detach();
        }
        else if (user_input == "exit")
        {
            std::cout << "Exiting editor for document: " << path << std::endl;
            break; // Exit the editing loop
        }
        else
        {
            std::cout << "User input: " << user_input << std::endl;
        }
    }
}

int main()
{

    while (!TracyIsConnected)
    {
        // Wait for the profiler to connect
    }

    ZoneScoped;
    edit_document("document1.txt");
    return 0;
}