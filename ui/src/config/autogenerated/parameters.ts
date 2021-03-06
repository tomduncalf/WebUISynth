// This file is autogenerated by the JUCE BrowserIntegrationPluginClient class,
// by inspecting all the parameters in the AudioProcessorValueTreeState.
//
// It is updated whenever you run the app in debug mode with the macro
// BROWSER_INTEGRATION_WRITE_PARAMETER_CONFIG_IN_DEBUG=1.
//
// You shouldn't need to edit this manually - if you add a new parameter,
// running the app in debug mode should be enough to update this.
        
import { ParameterModel } from "../../juceIntegration/models/ParameterModel";

export type ParametersType = {
  ampEnvAttack: ParameterModel<number>;
  ampEnvDecay: ParameterModel<number>;
  ampEnvRelease: ParameterModel<number>;
  ampEnvSustain: ParameterModel<number>;
  filterCutoff: ParameterModel<number>;
  filterResonance: ParameterModel<number>;
  osc1Type: ParameterModel<"Saw" | "Sine">;
};
        
export const PARAMETER_IDS = [
  "ampEnvAttack",
  "ampEnvDecay",
  "ampEnvRelease",
  "ampEnvSustain",
  "filterCutoff",
  "filterResonance",
  "osc1Type",
];
