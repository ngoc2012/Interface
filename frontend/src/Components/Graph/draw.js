
function setStyle(ctx, style, type) {
  let styleValue = style['default']
  let render = true
  if (Object.keys(style).includes(type)) {
    styleValue = style[type]
    if ("render" in styleValue) {
      render = styleValue.render
    }
  }
  ctx.setLineDash([]);
  for (let key of Object.keys(styleValue)) { 
    if (key === "dash") {
      ctx.setLineDash(styleValue[key]);
    } else {
      ctx[key] = styleValue[key]
    }
  }
  return render
}

export default function draw(ctx, graph, data) {

  const width = ctx.canvas.width;
  const height = ctx.canvas.height;

	ctx.clearRect(0, 0, width, height);
  ctx.save();
  
  // Canvas boundary
  // console.log(data.boundary)
  ctx.beginPath();
	ctx.lineTo(0, 0);
  ctx.lineTo(width, 0);
  ctx.lineTo(width, height);
  ctx.lineTo(0, height);
  ctx.lineTo(0, 0);
  ctx.stroke();

  ctx.translate(graph.offset.x, -graph.offset.y);
  
  // console.log(data.lines)
  // Lines

  
  if ("lines" in data) {
    for (let e of data.lines){
      let render = setStyle(ctx, graph.lineStyle, e.type)
      
      if (render) {
        ctx.beginPath();
        ctx.lineTo(e.start.x*graph.scaleX, height - e.start.y*graph.scaleY);
        ctx.lineTo(e.end.x*graph.scaleX, height - e.end.y*graph.scaleY);
        ctx.stroke();
      }
    }
  }
  
  
  // Circles
  if ("circles" in data) {
    for (let e of data.circles){
      let render = setStyle(ctx, graph.circleStyle, e.type)
      
      if (render) {
        ctx.beginPath();
        ctx.arc(e.x*graph.scaleX, height-e.y*graph.scaleY, e.d/2.0*(Math.min(graph.scaleX, graph.scaleY)), 0, 2 *Math.PI);
        ctx.stroke();
      }
    }
  }
  
  // Texts
  if ("texts" in data) {
    for (let e of data.texts){
      let render = setStyle(ctx, graph.textStyle, e.type)
      
      let x = (e.x + ctx.offsetCenter.x)*graph.scaleX;
      let y = height - (e.y + ctx.offsetCenter.y)*graph.scaleY;
      if (render) {
        if (e.angle !== 0) {
          // console.log(e.angle, e.str.toString())
          ctx.save();
          ctx.translate(x, y);
          ctx.rotate(e.angle*Math.PI/180.0);
          ctx.fillText(e.str.toString(), 0, 0);
          ctx.restore();
        } else {
          ctx.fillText(e.str.toString(), x, y);
        }
      }
    }
  }
    
  ctx.restore();

  
};
