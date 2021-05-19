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
    <div style={{ display: "flex", marginBottom: 2 }}>
      <div style={{ flex: 1 }}>{props.label}</div>
      <input
        type="range"
        min={props.min || 0}
        max={props.max || 1}
        step={props.step || 0.01}
        value={props.parameter.value}
        onChange={(e) => (props.parameter.value = Number(e.target.value))}
        style={{ flex: 2 }}
      />
    </div>
  ) : null;
});
