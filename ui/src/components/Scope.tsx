import { createRef, useRef } from "react";
import { registerCallback } from "../juceIntegration/juceCommunication";

export const Scope = () => {
  const canvasRef = createRef<HTMLCanvasElement>();

  registerCallback("Scope::scopeUpdate", (data: number[]) => {
    if (!canvasRef.current) return;

    const canvas = canvasRef.current;
    const context = canvasRef.current.getContext("2d")!;

    context.clearRect(0, 0, canvas.width, canvas.height);
    const gradient = context.createLinearGradient(
      0,
      0,
      canvas.width,
      canvas.height
    );
    gradient.addColorStop(0, "red");
    gradient.addColorStop(1, "blue");
    context.strokeStyle = gradient;
    context.lineWidth = 1;

    context.beginPath();
    context.moveTo(0, canvas.height / 2);
    for (let i = 0; i < data.length; i++) {
      const nextX = canvas.width * ((i + i) / data.length);
      context.lineTo(nextX, canvas.height / 2 + (data[i] * canvas.height) / 4);
    }
    context.stroke();
  });

  return <canvas ref={canvasRef} style={{ width: "100vw", height: 200 }} />;
};
