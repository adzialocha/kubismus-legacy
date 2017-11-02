#include "../JuceLibraryCode/JuceHeader.h"

#include "MainComponent.h"
#include "LookAndFeelCustom.h"

class KubismusApplication : public JUCEApplication
{
    
public:

    KubismusApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return false; }

    void initialise (const String& commandLine) override
    {
        mainWindow = new MainWindow (getApplicationName());
        
        LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);
    }

    void shutdown() override
    {
        mainWindow = nullptr; // (deletes our window)
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        if (! commandLine.trim().startsWithChar ('-'))
            mainWindow->openFile (File (commandLine.unquoted()));
    }

    class MainWindow    : public DocumentWindow,
                          public ApplicationCommandTarget,
                          public MenuBarModel,
                          private ChangeListener
    {
        
    public:
        
        MainWindow (String _name)  : DocumentWindow (_name,
                                                    Colours::lightgrey,
                                                    DocumentWindow::allButtons),
                                     menu(this),
                                     name(_name)
        {
            main = new MainContentComponent();
            
            main->addChangeListener (this);
            
            setUsingNativeTitleBar (true);
            setContentOwned (main, true);
            
            commandManager.registerAllCommandsForTarget (this);
            
            setApplicationCommandManagerToWatch (&commandManager);
            
            #if JUCE_MAC
                menu.setModel (nullptr);
                MenuBarModel::setMacMainMenu (this);
            #endif
            
            addKeyListener (commandManager.getKeyMappings());

            centreWithSize (getWidth(), getHeight());
            setResizable(true, false);
            setVisible (true);
            
            setName (name + " - " + main->getCurrentDocumentTitle());
        }
        
        ~MainWindow ()
        {
            #if JUCE_MAC
                MenuBarModel::setMacMainMenu (nullptr);
            #endif
            
            removeKeyListener (commandManager.getKeyMappings());
        }
        
        void openFile (File f)
        {
            main->openProjectFromFile (f);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }
        
        void changeListenerCallback (ChangeBroadcaster *source) override
        {
            setName (name + " - " + main->getCurrentDocumentTitle());
        }
        
        // ===============================================================
        
        // ApplicationCommandTarget
        
        enum CommandIDs
        {
            newProject            = 0x2000,
            openProject           = 0x2001,
            saveProject           = 0x2002,
            saveAsProject         = 0x2003,
            
            showPreferences       = 0x3000
        };
        
        ApplicationCommandTarget* getNextCommandTarget() override
        {
            return findFirstTargetParentComponent();
        }
        
        void getAllCommands (Array<CommandID>& commands) override
        {
            const CommandID ids[] = {
                MainWindow::newProject,
                MainWindow::openProject,
                MainWindow::saveProject,
                MainWindow::saveAsProject,
                
                MainWindow::showPreferences
            };
            
            commands.addArray (ids, numElementsInArray (ids));
        }
        
        void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override
        {
            const String fileCategory ("File");
            
            switch (commandID)
            {
                case MainWindow::newProject:
                    result.setInfo ("New", "Clears the current project for a new one", fileCategory, 0);
                    result.addDefaultKeypress ('N', ModifierKeys::commandModifier);
                    result.setActive (true);
                    break;
                    
                case MainWindow::openProject:
                    result.setInfo ("Open", "Open a saved project file", fileCategory, 0);
                    result.addDefaultKeypress ('O', ModifierKeys::commandModifier);
                    result.setActive (true);
                    break;
                    
                case MainWindow::saveProject:
                    result.setInfo ("Save", "Save a project or overwrite a current one", fileCategory, 0);
                    result.addDefaultKeypress ('S', ModifierKeys::commandModifier);
                    result.setActive (main->hasOpenProject());
                    break;
                    
                case MainWindow::saveAsProject:
                    result.setInfo ("Save As", "Save a project in a new file", fileCategory, 0);
                    result.setActive (true);
                    break;
                    
                case MainWindow::showPreferences:
                    result.setInfo ("Preferences", "Open preferences video", fileCategory, 0);
                    result.setActive (true);
                    break;
                    
                default:
                    break;
            }
        }
        
        bool perform (const InvocationInfo& info) override
        {
            switch (info.commandID)
            {
                case MainWindow::newProject:
                {
                    main->newProject();
                    break;
                }
                case MainWindow::openProject:
                {
                    main->openProject();
                    break;
                }
                case MainWindow::saveProject:
                {
                    main->saveProject();
                    break;
                }
                case MainWindow::saveAsProject:
                {
                    main->saveAsProject();
                    break;
                }
                case MainWindow::showPreferences:
                {
                    main->showPreferences();
                    break;
                }
                default:
                    return false;
            }
            
            return true;
        }
        
        // MenuBarModel
        
        StringArray getMenuBarNames() override
        {
            const char* const names[] = { "File", "Edit", nullptr };
            return StringArray (names);
        }
        
        PopupMenu getMenuForIndex (int menuIndex, const String& /*menuName*/) override
        {
            PopupMenu menu;
            
            if (menuIndex == 0)
            {
                menu.addCommandItem (&commandManager, MainWindow::newProject);
                menu.addSeparator();
                
                menu.addCommandItem (&commandManager, MainWindow::openProject);
                menu.addSeparator();
                
                menu.addCommandItem (&commandManager, MainWindow::saveProject);
                menu.addCommandItem (&commandManager, MainWindow::saveAsProject);
            }
            else if (menuIndex == 1)
            {
                menu.addCommandItem (&commandManager, MainWindow::showPreferences);
            }
            
            return menu;
        }
        
        void menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/) override
        {
            // EMPTY
        }
        
    private:
        
        MenuBarComponent menu;
        ApplicationCommandManager commandManager;
                
        String name;
        
        MainContentComponent* main;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
        
    };

private:
    
    LookAndFeelCustom lookAndFeel;
    
    ScopedPointer<MainWindow> mainWindow;
    
};

START_JUCE_APPLICATION (KubismusApplication)
