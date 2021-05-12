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
    <div>
      {props.label}
      {props.choices.map((choice, index) => (
        <button onClick={(e) => ((props.parameter.value as any) = index)}>
          {choice}
        </button>
      ))}
    </div>
  ) : null;
});
