import { useContext } from "react";
import { ParametersContext } from "../juceIntegration/JuceIntegration";
import { ParameterChoice } from "./ParameterChoice";
import { ParameterSlider } from "./ParameterSlider";

export const Parameters = () => {
  const parameters = useContext(ParametersContext)!;

  return (
    <div style={{ padding: 10 }}>
      <ParameterChoice
        choices={["Saw", "Sin"]}
        label="Osc Waveform"
        parameter={parameters.osc1Type}
      />

      <ParameterSlider
        min={20}
        max={10000}
        parameter={parameters.filterCutoff}
        label="Filter Cutoff"
      />
      <ParameterSlider
        min={0}
        max={1}
        parameter={parameters.filterResonance}
        label="Filter Resonance"
      />
      <ParameterSlider
        min={0.001}
        max={2}
        parameter={parameters.ampEnvAttack}
        label="Amp Env Attack"
      />
      <ParameterSlider
        min={0}
        max={2}
        parameter={parameters.ampEnvDecay}
        label="Amp Env Decay"
      />
      <ParameterSlider
        min={0}
        max={1}
        parameter={parameters.ampEnvSustain}
        label="Amp Env Sustain"
      />
      <ParameterSlider
        min={0.001}
        max={2}
        parameter={parameters.ampEnvRelease}
        label="Amp Env Release"
      />
    </div>
  );
};
