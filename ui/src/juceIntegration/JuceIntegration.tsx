import React, { FC, useEffect, useRef, useState } from "react";
import {
  makeConnectedParametersModel,
  ParametersModelType,
} from "./models/ParametersModel";

export const ParametersContext =
  React.createContext<ParametersModelType | undefined>(undefined);

export const JuceIntegration: FC = (props) => {
  const [ready, setReady] = useState(false);
  const parameters = useRef<ParametersModelType | undefined>();

  useEffect(() => {
    (async () => {
      parameters.current = await makeConnectedParametersModel();
      setReady(true);
    })();
  }, []);

  return (
    <ParametersContext.Provider value={parameters.current}>
      {ready && props.children}
    </ParametersContext.Provider>
  );
};
