import "./App.css";
import { observer } from "mobx-react";
import { JuceIntegration } from "./juceIntegration/JuceIntegration";
import { Parameters } from "./components/Parameters";

const App = observer(() => {
  return (
    <JuceIntegration>
      <Parameters />
    </JuceIntegration>
  );
});

export default App;
