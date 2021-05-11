import "./App.css";
import { observer } from "mobx-react";
import { ValueTree } from "./valueTree/ValueTree";
import { useEffect } from "react";
import { InputStream } from "./valueTree/InputStream";
import { applyChange } from "./valueTree/ValueTreeSynchroniser";
import {
  registerCallback,
  sendMessageToJuce,
} from "./juceIntegration/juceIntegration";
import { toJS } from "mobx";
import { ParametersModel } from "./models/ParametersModel";
import { ParameterSlider } from "./components/ParameterSlider";

const valueTree = new ValueTree();
const parameters = new ParametersModel(valueTree);

const App = observer(() => {
  useEffect(() => {
    registerCallback("ValueTreeStateChange", (message: any) => {
      applyChange(
        valueTree,
        Uint8Array.from(atob(message.change), (c) => c.charCodeAt(0))
      );
      console.log(toJS(valueTree));
    });

    sendMessageToJuce({ eventType: "Main::AppReady" });
  }, []);

  const cutoff = valueTree.children.find(
    (c) => c.properties.get("id") === "filterCutoff"
  );

  return (
    <>
      <div className="App">{valueTree.type}</div>
      <ParameterSlider
        min={20}
        max={10000}
        parameter={parameters.filterCutoff}
        label="Filter cutoff"
      />
      <ParameterSlider
        min={0}
        max={1}
        parameter={parameters.filterResonance}
        label="Filter resonance"
      />
    </>
  );
});

export default App;
