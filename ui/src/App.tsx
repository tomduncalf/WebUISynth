import "./App.css";
import { observer } from "mobx-react";
import { JuceIntegration } from "./juceIntegration/JuceIntegration";
import { Parameters } from "./components/Parameters";
import { Scope } from "./components/Scope";

const App = observer(() => {
  return (
    <JuceIntegration>
      <Parameters />
      <Scope />
    </JuceIntegration>
  );
});

export default App;
