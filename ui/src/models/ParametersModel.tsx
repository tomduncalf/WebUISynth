import { makeAutoObservable, toJS } from "mobx";
import { ValueTree } from "../valueTree/ValueTree";

export type ParameterModel =
  | {
      id: string;
      value: number;
    }
  | undefined;

export class ParametersModel {
  constructor(private valueTree: ValueTree) {
    makeAutoObservable(this);
  }

  get filterCutoff(): ParameterModel {
    return this.getParameterModel("filterCutoff");
  }

  get filterResonance(): ParameterModel {
    return this.getParameterModel("filterResonance");
  }

  private getParameterModel = (id: string) => {
    const parameter = this.valueTree.childrenWithProperty("id", id);
    console.log(id, toJS(parameter));

    if (!parameter) return undefined;

    return {
      id: parameter.properties.get("id") as string,
      value: parameter.properties.get("value") as number,
    };
  };
}
