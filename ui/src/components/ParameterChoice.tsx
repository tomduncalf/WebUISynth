import { observer } from "mobx-react";
import { FC } from "react";
import { ParameterModel } from "../juceIntegration/models/ParameterModel";

interface Props {
  choices: string[];
  label: string;
  parameter: ParameterModel<string>;
}

export const ParameterChoice: FC<Props> = observer((props) => {
  return props.parameter ? (
    <div style={{ display: "flex" }}>
      <div style={{ flex: 1 }}>{props.label}</div>

      <div style={{ flex: 2 }}>
        {props.choices.map((choice, index) => (
          <button
            // TOOD proper typing of choice parameters - JUCE actually outputs an int not a string
            style={
              (props.parameter.value as any) === index
                ? { fontWeight: "bold" }
                : {}
            }
            onClick={(e) => ((props.parameter.value as any) = index)}
          >
            {choice}
          </button>
        ))}
      </div>
    </div>
  ) : null;
});
