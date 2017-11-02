#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "ExtendedViewport.h"
#include "ChannelViewComponent.h"
#include "SidebarComponent.h"
#include "TimelineComponent.h"
#include "ScannerManagerComponent.h"
#include "Session.h"
#include "DataInterface.h"
#include "ProjectDocument.h"
#include "PreferencesWindow.h"

class MainContentComponent   : public Component,
                               public DragAndDropContainer,
                               public ChangeBroadcaster,
                               public ChannelListener,
                               private KeyListener,
                               private ActionListener,
                               private ChangeListener,
                               private ViewportListener
{
    
public:

    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;
    
    // ViewportListener
    
    void visibleAreaChanged ( const Rectangle< int > &newVisibleArea ) override;
    
    // ChannelListener
    
    void channelSelected ( ChannelComponent &selectedChannel ) override;
    void channelElementSelected ( ChannelComponent &selectedChannel, ChannelElementComponent &selectedElement ) override;
    
    // ChangeListener
    
    void changeListenerCallback ( ChangeBroadcaster *source ) override;
    
    // ===============================================================
    
    // project
    
    void newProject ()
    {
        resetProject();
        document.reset();
        
        sendChangeMessage();
    }
    
    void openProject ()
    {
        document.loadFromUserSpecifiedFile (true);
        
        if (document.getFile() != File::nonexistent)
        {
            resetProject ();
            loadProject (document.getProjectRoot());
        }
        
        sendChangeMessage();
    }
    
    void openProjectFromFile (File f)
    {
        document.loadFrom (f, true);
        
        if (document.getFile() != File::nonexistent)
        {
            resetProject();
            loadProject (document.getProjectRoot());
        }
        
        sendChangeMessage();
    }
    
    void saveProject ()
    {
        session.stop();
        
        document.setProjectRoot (createProject());
        document.save (true, true);
        
        sendChangeMessage();
    }
    
    void saveAsProject ()
    {
        session.stop();
        
        document.setProjectRoot (createProject());
        document.saveAsInteractive (true);
        
        sendChangeMessage();
    }
    
    bool hasOpenProject ()
    {
        return document.getLastDocumentOpened() != File::nonexistent;
    }
    
    String getCurrentDocumentTitle ()
    {
        return document.getDocumentTitle();
    }
    
    // preferences window
    
    void showPreferences ()
    {
        preferences.setVisible(true);
    }
    
private:
    
    const int MINUTE_IN_PIXELS = 2000;
    
    const int TIMELINE_HEIGHT = 25;

    const int SIDEBAR_WIDTH = 200;
    const int SIDEBAR_WIDTH_COLLAPSED = 25;
    
    bool isTransportActive;
    
    int scannerStart;
    
    ProjectDocument document;
    
    ExtendedViewport viewport;
    
    ChannelViewComponent channelView;
    SidebarComponent sidebar;
    
    TimelineComponent timeline;
    ScannerManagerComponent manager;
    
    Session session;
    DataInterface interface;
    
    PreferencesWindow preferences;
    
    // ===============================================================
    
    void resetProject ()
    {
        session.stop();
        session.setPosition(0);
        
        sidebar.reset();
        channelView.reset();
        manager.reset();
    }
    
    void loadProject (ValueTree project)
    {
        for (unsigned i = 0; i < project.getNumChildren(); i++)
        {
            if (project.getChild(i).hasType("CHANNELS"))
                channelView.loadFromDocument (project.getChild(i));
            else if (project.getChild(i).hasType("SCANNERS"))
                manager.loadFromDocument (project.getChild(i));
        }
    }
    
    ValueTree createProject ()
    {
        ValueTree project ("KUBISMUS");
        
        project.addChild (channelView.getForDocument(), 0, nullptr);
        project.addChild (manager.getForDocument(), 1, nullptr);
        
        return project;
    }
    
    // ==============================================================
    
    void connectToServer ()
    {
        if (! interface.connect(preferences.getAddress(), preferences.getPort()))
        {
            sidebar.setConnectionState(false);
            
            AlertWindow::showMessageBoxAsync (
                                              AlertWindow::WarningIcon,
                                              "Connection error",
                                              "Something went wrong",
                                              "OK");
        }
        else
        {
            sidebar.setConnectionState(true);
        }
    }
    
    // ===============================================================

    // KeyListener
    
    bool keyPressed (const juce::KeyPress &key, juce::Component *originatingComponent) override;
    
    // ActionListener
    
    void actionListenerCallback	( const String & message ) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
