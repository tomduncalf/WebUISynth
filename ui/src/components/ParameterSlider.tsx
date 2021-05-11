import { FC } from "react";
import { sendMessageToJuce } from "../juceIntegration/juceIntegration";
import { ParameterModel } from "../models/ParametersModel";

interface Props {
  min?: number;
  max?: number;
  step?: number;
  label: string;
  parameter: ParameterModel;
}

export const ParameterSlider: FC<Props> = (props) => {
  return props.parameter ? (
    <div>
      {props.label}
      <input
        type="range"
        min={props.min || 0}
        max={props.max || 1}
        step={props.step || 0.01}
        value={props.parameter.value}
        onChange={(e) =>
          sendMessageToJuce({
            eventType: "Main::setParameter",
            data: {
              id: props.parameter!.id,
              value: Number(e.target.value),
            },
          })
        }
      />
    </div>
  ) : null;
};
