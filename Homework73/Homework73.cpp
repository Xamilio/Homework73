#include <iostream>
#include <string>
#include <memory>
using namespace std;

class IDocument {
public:
    virtual ~IDocument() = default;
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void read() = 0;
};

class RealDocument : public IDocument {
private:
    string filename;
    bool loaded;
    string content;

    void load() 
    {
        cout << "Loading document '" << filename << "'...\n";
        content = "This is the content of the document.";
        loaded = true;
        cout << "Document '" << filename << "' loaded.\n";
    }
public:
    RealDocument(const string& filename) : filename(filename), loaded(false) 
    {

    }

    void open() override 
    {
        if (!loaded) 
        {
            load();
        }
        cout << "Document '" << filename << "' is opened.\n";
    }

    void close() override 
    {
        cout << "Document '" << filename << "' is closed.\n";
    }

    void read() override 
    {
        if (!loaded) 
        {
            load();
        }
        cout << "Reading content: " << content << '\n';
    }
};

class DocumentProxy : public IDocument {
private:
    string filename;
    unique_ptr<RealDocument> realDocument;

public:
    DocumentProxy(const string& filename) : filename(filename), realDocument(nullptr) {}

    void open() override 
    {
        if (!realDocument) 
        {
            realDocument = make_unique<RealDocument>(filename);
        }
        realDocument->open();
    }

    void close() override 
    {
        if (realDocument) 
        {
            realDocument->close();
        }
    }

    void read() override 
    {
        if (!realDocument) 
        {
            realDocument = make_unique<RealDocument>(filename);
        }
        realDocument->read();
    }
};

void client(IDocument& document) 
{
    document.open();
    document.read();
    document.close();
}

int main() {
    DocumentProxy proxy("test_document.txt");
    client(proxy);
}