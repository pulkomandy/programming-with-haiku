Learning to Program with Haiku

Lesson 14

Written by DarkWyrm

All material © 2010 DarkWyrm

If you've been waiting excitedly for the chance to learn how to write a program that shows something on the screen instead of just printing stuff on the Terminal, today's the day! We will spend most of our time understanding the code, but let's talk about tools and organization first.

Tools of the Trade: Integrated Development Environments {#_tools_of_the_trade_integrated_development_environments}
-------------------------------------------------------

All of our programs so far have been simple. Console programs, as they are called, don't have to be, but to learn the language, simple is good. A text editor is all we've needed to do these simple programs which can all fit into one file. Regular applications can have hundreds of files. Many experienced C++ programmers are content with a text editor and Terminal, but having more isn't a bad thing, especially for novice coders.

For the rest of these lessons, you will be encouraged -- but not required, however -- to use an Integrated Development Environment (IDE). IDEs tie together the compiler, the code editor, the debugger, and other tools so that a developer can work more efficiently. For the purposes of these lessons, use of the Paladin IDE for Haiku will be assumed. If you prefer using another one, that's just fine. Let's get started.

Our First Project {#_our_first_project}
-----------------

![Illustration 1: The Paladin New Project window](Pictures/100000000000019B000000EBFE98E9A4.png)Run Paladin and you will be greeted by the start window. Choose to create a new project. At the top of the New Project window is a menu where you can choose what kind of project to create. Choose Empty Application. While we could choose GUI with Main Window to automate what we are about to do, the act of typing the source code will help us learn the basics of Haiku GUI programming better. Set your project name to whatever you like. The Target Name is the name that the executable will have when your project is built and is often the same as the project name. When you're ready to move on, click Create Project.

As of this writing, if you are using a GCC4-based build of Haiku, you will need to add a library to your project to be able to build it properly. In your project's window, choose Change System Libraries from the Project menu. Scroll down and check the box beside libsupc++.so. Close the window and from the Project menu, choose Add New File. When asked the name of your file, enter App.cpp, check the box to create the corresponding header file, and click OK.

In general, there is one class per pair of files and the base of the files' names is the same as the class that goes in it. Our first file, App.cpp will have one class: App. The definition for the App class will go into the header file App.h. All of the code for the class will go into App.cpp. If we had a couple of really small related classes, it would be acceptable to put them both into the same file, but the general rule of thumb is one class per file pair for organization's sake.

Let's start with adding what we need to App.h. Click ONCE on App.cpp in the project window to select it, hold Alt and press Tab. You'll notice that App.h appears in the editor. The Alt-Tab key combination is a fast way of jumping between a file's header and its source file, but it only works if both files have the same base name, such as App.cpp and App.h. Start by entering the code below:

\#ifndef APP\_H

\#define APP\_H

\#include \<Application.h\>

class App : public BApplication

\\{

public:

App(void);

};

\#endif

Every GUI program in the Haiku operating system creates a subclass of BApplication. This class sets up communications with the app\_server, the part of the system responsible for actually drawing everything on the screen, handling font information, and a host of other tasks. Without this connection, we can't do very much.

Save your work if you haven't done so already and switch to App.cpp (Alt + Tab) to enter in this code:

\#include "App.h"

\#include \<StringView.h\>

\#include \<Window.h\>

App::App(void)

:BApplication("application/x-vnd.dw-MyFirstApp")

\\{

BRect frame(100,100,500,400);

BWindow \*myWindow = new BWindow(frame,"My First App",B\_TITLED\_WINDOW,

B\_QUIT\_ON\_WINDOW\_CLOSE);

frame.Set(10,10,11,11);

BStringView \*label = new BStringView(frame,"mylabel","Haiku Rocks!");

label→ResizeToPreferred();

myWindow→AddChild(label);

myWindow→Show();

}

int

main(void)

\\{

App \*app = new App();

app→Run();

delete app;

return 0;\
}

Save your work and either click on Run from the Build menu or press Alt + R to build and run your project. If you've typed everything correctly, you should see this on the screen:

![image](Pictures/100000000000019C0000014DB82143E1.png)

If you run into errors, carefully check to make sure that you've typed everything just the same as the code above.

We've written our first working Haiku app! Not only does it show something on the screen, but we can resize the window, hide it, or close it and quit the program. It's not terribly useful, but that's OK. We had to learn a lot just to get this far and still understand what's going on.

One small bit of code was not explained in the comments, namely these two lines:

App::App(void)

:BApplication("application/x-vnd.dw-MyFirstApp")

The part inside the quotation marks doesn't seem at all familiar. This is what's called a MIME type. MIME stands for **M**ultipurpose **I**nternet **M**ail **E**xtensions. Every file in Haiku has a type, including all Haiku programs. Graphical Haiku programs like ours have a unique type. Substitute your company's name, your online handle, or your initials for the *dw* and the name of your program for *MyFirstApp* -- just make sure that the signature begins with \_\"application/x-vnd.\" \_ This will ensure no mixups with any programs you publish online.

This project was a barebones application to show the minimum code needed to create a simple window. It might seem like a lot, but in comparison to a Windows program written in C++, it's quite short. Next time, we'll make a window that does more, but for now, play around with the code and see what kinds of things you can do with it -- tinkering leads to better understanding.
