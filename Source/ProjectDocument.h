#ifndef PROJECTDOCUMENT_H_INCLUDED
#define PROJECTDOCUMENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class ProjectDocument : public FileBasedDocument

{
public:
    
    ProjectDocument() : FileBasedDocument(
                            ".miau",
                            "*.miau",
                            "Choose a project file to open..",
                            "Choose a project file to save as.."
                        ),
                        projectRoot (ROOT_KEY)
    {
        lastDocumentOpened = File::nonexistent;
    };
    
    ~ProjectDocument()
    {

    };
    
    void reset ()
    {
        setFile (File());
        
        projectRoot = ValueTree (ROOT_KEY);
        
        setChangedFlag (false);
        
        setLastDocumentOpened (File());
    }
    
    ValueTree getProjectRoot() const { return projectRoot; }
    void setProjectRoot (ValueTree data) { projectRoot = data; }
    
    File getLastDocumentOpened() { return lastDocumentOpened; }
    void setLastDocumentOpened (const File& file) { lastDocumentOpened = file; }
    
    String getDocumentTitle()
    {
        if (! getFile().exists())
            return "Untitled";
        
        return getFile().getFileNameWithoutExtension();
    }
    
    Result loadDocument (const File& file)
    {
        ScopedPointer<XmlElement> xml (XmlDocument::parse (file));
        
        if (xml == nullptr || ! xml->hasTagName (ROOT_KEY))
            return Result::fail ("Not a valid kubismus project!");
        
        ValueTree newTree (ValueTree::fromXml (*xml));
        
        if (! newTree.hasType (ROOT_KEY))
            return Result::fail ("The document contains errors and couldn't be parsed!");
        
        projectRoot = newTree;
        
        setChangedFlag (false);
        
        return Result::ok();
    }
    
    Result saveDocument (const File& file)
    {
        projectRoot.setProperty ("version", ProjectInfo::versionString, nullptr);
        
        ScopedPointer<XmlElement> xml (projectRoot.createXml());
        
        if (! xml->writeToFile (file, String::empty))
            return Result::fail ("Couldn't write to the file");
        
        return Result::ok();
    }
    
private:
    
    File lastDocumentOpened;
    
    const String ROOT_KEY = "KUBISMUS";
    
    ValueTree projectRoot;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectDocument)
    
};


#endif  // PROJECTDOCUMENT_H_INCLUDED
