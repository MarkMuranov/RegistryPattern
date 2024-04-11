# Registry Pattern
ECE 141B

## Motivation

When setting up the Chain of Responsibilities (COR) pattern, you have to
create each link/handler in the chain and then connect them. 
Here is an example:

```cpp
// Set up the COR
auto basicHandler = std::make_shared<BasicCommandHandler>();
auto databaseHandler = std::make_shared<DatabaseCommandHandler>();
auto tableHandler = std::make_shared<TableCommandHandler>();
basicHandler->setNext(databaseHandler);
databaseHandler->setNext(tableHandler);

// Use the COR
basicHandler->find(tokenizer);
```

This is bad for two reasons:
1. **Tedious:** For each Handler you add, you have to come back here and 
modify the COR set up code
2. **Error-prone:** It is easy to incorrectly connect the handlers by 
calling `setNext()` incorrectly.

## Simplifying using Singletons

To improve this design, instead of manually connecting the handlers, we
can create some centralized COR controller to take care of this for us.

Let's create a singleton, which will have a method to add a new handler
to the chain, `registerHandler()` and a method to use the COR, `handle()`:

```cpp
class ChainOfResponsibilities {
private:
    ChainOfResponsibilities() = default;

public:
    ChainOfResponsibilities(const ChainOfResponsibilities&) = delete;
    ChainOfResponsibilities& operator=(const ChainOfResponsibilities&) = delete;

    // Get singleton instance
    static ChainOfResponsibilities& getInstance() {
        static ChainOfResponsibilities instance;
        return instance;
    }

    // Add new handler to the COR
    template<class HandlerType>
    void registerHandler() {
        auto newHandler = std::shared_ptr<Handler>(new HandlerType);
        if (!firstHandler) {
            firstHandler = newHandler;
            lastHandler = newHandler;
        }
        else {
            lastHandler->setNext(newHandler);
            lastHandler = newHandler;
        }
    }

    // Use the COR
    bool handle(Tokenizer& tokenizer) {
        if (!firstHandler)
            return false;

        return firstHandler->find(tokenizer);
    }

private:
    std::shared_ptr<Handler> firstHandler, lastHandler;

};
```

The `registerHandler()` method is a template method. In order to use this
method, you must pass in the specific Handler class you want to add
to the chain. This method will create a new `shared_ptr` instance of the 
specific `HandlerType`. Here is an example:

```cpp
using COR = ChainOfResponsibilities;
COR::getInstance().registerHandler<DatabaseCommandHandler>();
```

If this is the first Handler being registered, then it will be set
to the `firstHandler`. Otherwise, the `setNext()` method will be called on
the `lastHandler`, after which the `lastHandler` is updated to this new
Handler.

This is already a huge improvement for two reasons:
1. This eliminates the potential for mistakes when connecting the Handlers
in the chain using `setNext()`.
2. We can add new Handlers to the COR from anywhere in our code, as long
as we can access the `ChainOfResponsibilities` singleton.

So now our example to set up the COR looks like this:

```cpp
// Set up the COR
auto& CORInstance = ChainOfResponsibilities::getInstance();
CORInstance.registerHandler<BasicCommandHandler>();
CORInstance.registerHandler<DatabaseCommandHandler>();
CORInstance.registerHandler<TableCommandHandler>();

// Use the COR
CORInstance.handle(tokenizer);
```

Already this makes our code easier to follow, since all the connection logic
(calling `setNext()`) is done for us by this singleton. However,
we can do better >:D

## Registry Pattern

Finally, we get to the **Registry Pattern**. Ideally, we would be able to
write code that gets run at the start of the program's runtime from within
any file (without having to explicitly call that code from the start of
the program). However, unlike Python, we cannot just write code in the
global scope (you will get a compile error).

But there is a workaround. While we can't write code in the global scope,
we *can* declare variables in the global scope, including objects; these
objects will get constructed at the beginning of the program, even before
the `main()` function is run. And what gets called whenever an object is
constructed?
    
> Hint: It rhymes with "conductor".

This means that we can create an object just to hijack its constructor
to execute any code we want! Let's go ahead and create a `RegisterHandler`
class which call the `ChainOfResponsibilty`'s `registerHandler()` method
in its constructor:

```cpp
template<class HandlerType>
struct RegisterHandler {
    RegisterHandler() {
        ChainOfResponsibilities::getInstance().registerHandler<HandlerType>();
    }
};
```

Just like the `registerHandler()` method, this class utilizes templates.
In this case, the template argument is just forwarded to the method.

In the file where we actually declare the various Handler classes (like
`BasicCommandHandler`, `DatabaseCommandHandler`, ...), we can declare
a global instance of the `RegisterHandler` class at the bottom of these
files, like so:

```cpp
inline const RegisterHandler<BasicCommandHandler> registerBasicHandler;
inline const RegisterHandler<DatabaseCommandHandler> registerDatabaseHandler;
inline const RegisterHandler<TableCommandHandler> registerTableHandler;
```

> Note: The `inline` keyword is used to ensure that these object are only
> created once.

Then, from where we actually want to use the COR, all we have to do is:

```cpp
ChainOfResponsibilities::getInstance().handle(tokenizer);
```

With this design, we can add new Handlers from anywhere in our code, from
any file! If you have some centralized controller which was setting up the
COR, you can now delete that code! In fact, anytime you add a new Handler
these controllers do not have to be modified/updated at all!

> Note: You may have to `#include ...` the corresponding file where
> the Handler was defined and registered to ensure that code actually
> runs.

## This Repository

In this repository, you will find a very simple example of how this
design (along with the COR pattern) can be used! A lot of components, like
the `Tokenizer` and the individual Handlers have very bare-bones
implementations, as to not add noise to what's important. Feel free
to expand/replace these with more robust implementations!

The `Rick.h` file has our professor's implementation of the COR pattern for
easy comparison to mine.