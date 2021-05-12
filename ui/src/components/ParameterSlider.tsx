import { observer } from "mobx-react";
import { FC } from "react";
import { ParameterModel } from "../juceIntegration/models/ParameterModel";

interface Props {
  min?: number;
  max?: number;
  step?: number;
  label: string;
  parameter: ParameterModel<number>;
}

export const ParameterSlider: FC<Props> = observer((props) => {
  return props.parameter ? (
    <div>
      {props.label}
      <input
        type="range"
        min={props.min || 0}
        max={props.max || 1}
        step={props.step || 0.01}
        value={props.parameter.value}
        onChange={(e) => (props.parameter.value = Number(e.target.value))}
      />
    </div>
  ) : null;
});
