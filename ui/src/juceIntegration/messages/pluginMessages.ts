import { sendMessageToJuce } from "../juceCommunication";
import { JuceVariant } from "../valueTree/InputStream";

export const setParameter = <T extends JuceVariant>(id: string, value: T) => {
  sendMessageToJuce({
    eventType: "Plugin::setParameter",
    data: {
      id,
      value,
    },
  });
};

export const initPlugin = () => {
  sendMessageToJuce({ eventType: "Plugin::init" });
};
