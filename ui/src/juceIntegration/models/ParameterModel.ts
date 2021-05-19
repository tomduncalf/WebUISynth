import { makeAutoObservable } from "mobx";
import { setParameter } from "../messages/pluginMessages";
import { JuceVariant } from "../valueTree/InputStream";
import { ValueTree } from "../valueTree/ValueTree";

export class ParameterModel<T extends JuceVariant> {
  constructor(private parameter: ValueTree) {
    makeAutoObservable(this);
  }

  get id() {
    return this.parameter.properties.get("id") as string;
  }

  get value() {
    return this.parameter.properties.get("value") as T;
  }

  set value(value: T) {
    setParameter(this.id, value);
  }
}
