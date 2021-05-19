# WebUISynth

A project building a simple synth (using JUCE's DSP classes and mostly copying the [JUCE DSP tutorial](https://docs.juce.com/master/tutorial_dsp_introduction.html)) integrated with a web UI (using React and Typescript). The UI is automatically synchronised with the plugin's state, taking advantage of JUCE's ValueTree class on the C++ side and MobX on the Javascript side.

<img alt="Screenshot of WebUISynth running" src="https://user-images.githubusercontent.com/5458070/118700820-f6849580-b80a-11eb-97ef-b5624b6d50c0.png" width="500" />

The project is intended as a showcase for the [juce_browser_integration](https://github.com/tomduncalf/juce_browser_integration) module I have created, which provides helper classes and patterns for easily integrating a web UI with a JUCE app.

You can see that only a small amount of code is required from the developer to hook up UI to the plugin's parameters: https://github.com/tomduncalf/WebUISynth/blob/main/ui/src/components/Parameters.tsx – the code in https://github.com/tomduncalf/WebUISynth/tree/main/ui/src/juceIntegration (which will be split out into its own reusable npm module) takes care of automatically synchronising state between JUCE and JS.

The plugin also integrates an oscilloscope running at 30fps (rendered using a HTML canvas), to demonstrate that fast state updates are possible without a significant impact on performance – in my measurements, the CPU usage remained in the same ballpark as doing the same rendering with JUCE graphics.

Please see the JUCE module's repo for more details on the integration: https://github.com/tomduncalf/juce_browser_integration.

Ultimately this project and the integration will be cross-platform on both desktop and mobile, but right now it has only been tested on macOS and iOS (where it currently works standalone but not as an AUv3).

## Status

Work in progress - basic engine and UI integration is working, but needs tidy up, styling, modularisation, documenting and a tutorial writing.

## Requirements

- JUCE v6 (https://juce.com/)
- Node.js (I recommend installing it with https://github.com/nvm-sh/nvm to allow multiple versions to co-exist)
- Xcode 12 (install from the Mac App Store)

## Running the project

1. Clone this repository
2. Run `git submodule update --init` to clone the required submodules (JUCE itself, and the juce_browser_integration module)
3. Open `WebUISynth.jucer` in Projucer (which should have been installed with JUCE)
4. Ensure the `Xcode (macOS)` exporter is selected in the header and press the "Xcode icon" button to export a new Xcode project
5. Select the `WebUISynth - Standalone Plugin` target in Xcode's header, then click the build button (play icon). The project should compile and start running as a standalone application. It should also open a Terminal window, in which the React dev server will run (so that the UI can be loaded from your local machine) – check this has no errors.
6. Go to `Options` > `Audio/MIDI Settings` and select your MIDI keyboard as an input and play some notes – you should hear a basic synth sound which you can modify with the sliders.

You can now edit the UI code in the `ui` directory (I recommend using VS Code as an editor as it has excellent Typescript support) and you should see the changes instantly in your app.

You should also be able run to the synth as a plugin – I find this a bit less convenient for developing, as your plugin host probably takes a little while to startup. You may need to resign your plugin host to allow you to debug plugins running inside it. For Ableton Live you can do this by:

1. Copy the script at https://gist.github.com/talaviram/1f21e141a137744c89e81b58f73e23c3 into a new file, `add_debug_entitlement.sh`
2. Run `./add_debug_entitlement.sh /Applications/Ableton\ Live\ 11\ Suite.app`. If you get errors, you may need to remove some existing attributes on Ableton with `xattr -cr /Applications/Ableton\ Live\ 11\ Suite.app`, then rerun the script.

## Accessing web developer tools

By default, the JUCE WebBrowserComponent does not expose the ability to access the Safari developer tools, which are really useful when debugging your UI. I intend to work with the JUCE team to expose this functionality in JUCE, but for now you need to manually patch the browser component:

1. In Xcode, open `JUCE Modules` > `juce_gui_extra/native/juce_mac_WebBrowserComponent.mm` (you can press Shift-Cmd-O and start typing the name to quickly open it in Xcode).
2. Scroll to line 275 (`WKWebViewConfiguration* config = [[WKWebViewConfiguration alloc] init];`).
3. Add a line below this, `[config.preferences setValue:@YES forKey:@"developerExtrasEnabled"];`
4. Now when you recompile the app, you can right click the UI and choose `Inspect Element` to open the developer tools.

## TODO

- [ ] Move JUCE integration code into a reusable npm module
- [ ] Proper styling
- [ ] also see https://github.com/tomduncalf/tomduncalf_juce_browser_integration/#TODO for wider integration TODO's
