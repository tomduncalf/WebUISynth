const callbacks = new Map<string, Function[]>();

export interface JuceMessage<T> {
  eventType: string;
  data?: T;
}

export const registerCallback = <T extends Function>(
  eventType: string,
  handler: T
) => {
  const existingCallbacks = callbacks.get(eventType);

  if (existingCallbacks) existingCallbacks.push(handler);
  else callbacks.set(eventType, [handler]);
};

export const sendMessageToJuce = <T>(message: JuceMessage<T>) => {
  try {
    window.location.href = "juce://" + JSON.stringify(message);
  } catch (e) {
    console.error("Error sending message to JUCE", { e, message });
  }
};

declare global {
  interface Window {
    receiveMessageFromJuce: any;
  }
}

window.receiveMessageFromJuce = (message: JuceMessage<any>) => {
  console.log("receiveMessageFromJuce", message);

  try {
    // const message: JuceMessage<unknown> = JSON.parse(message);
    const existingCallbacks = callbacks.get(message.eventType);

    if (existingCallbacks) {
      existingCallbacks.forEach((cb) => cb(message.data));
    } else {
      console.log(
        `No callbacks registered for event type "${message.eventType}"`,
        { message }
      );
    }
  } catch (e) {
    console.error("Error handling message from JUCE", { e, message });
    throw e;
  }
};
