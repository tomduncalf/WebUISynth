#include "MainComponent.h"

//==============================================================================
class Sync : public juce::ValueTreeSynchroniser
{
public:
    Sync (const juce::ValueTree& tree) : juce::ValueTreeSynchroniser (tree) {};
    
    void stateChanged(const void *encodedChange, size_t encodedChangeSize) override
    {
//        juce::MemoryBlock b { encodedChange, encodedChangeSize };

        auto file = juce::File ("/tmp/sync");
        file.deleteFile();
    
        juce::FileOutputStream o { file };
        o.write(encodedChange, encodedChangeSize);
        o.flush();
    }
};

MainComponent::MainComponent()
{
    setSize (600, 400);

    juce::Array<juce::var> array { juce::var{"Test"}, juce::var{1234}, juce::var{true} };

    juce::ValueTree tree {"TestTree"};
    tree.setProperty("stringProperty", "Test", nullptr);
    tree.setProperty("intProperty", 1234, nullptr);
    tree.setProperty("int64Property", (int64_t) 9223372036854775800, nullptr);
    tree.setProperty("doubleProperty", 0.1234567, nullptr);
    tree.setProperty("boolProperty", true, nullptr);
    tree.setProperty("arrayProperty", array, nullptr);
    
    juce::ValueTree childTree1 {"TestChild1"};
    childTree1.setProperty("stringProperty", "TestChildProperty", nullptr);
    tree.addChild(childTree1, 0, nullptr);
    
    juce::ValueTree nestedChildTree1 {"TestNestedChild1"};
    nestedChildTree1.setProperty("stringProperty", "TestNestedChildProperty", nullptr);
    childTree1.addChild(nestedChildTree1, 0, nullptr);
    
    juce::ValueTree childTree2 {"TestChild2"};
    tree.addChild(childTree2, 1, nullptr);
    
    Sync s { tree };

//    tree.setProperty("stringProperty", "Test 2", nullptr);
//    tree.removeProperty("stringProperty", nullptr);
//    juce::ValueTree childTree3 {"TestChild3"};
//    tree.addChild(childTree3, 0, nullptr);
//    tree.addChild(childTree3, 1, nullptr);
//    tree.addChild(childTree3, 2, nullptr);
//    tree.removeChild(childTree2, nullptr);
    tree.moveChild(0, 1, nullptr);
    
//    auto file = juce::File ("/tmp/mo");
//    file.deleteFile();
//
//    juce::FileOutputStream o { file };
//    tree.writeToStream(o);
//    o.flush();
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
