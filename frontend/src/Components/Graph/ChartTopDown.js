import React from 'react';
import Graph from '../Graph/Graph';

function subplot(props) {

  let dataGraph ={
    lines: [],
    texts: []
  }
  
  
  let graph = props.graph
  // console.log(props.data)
  let Xmax = Math.max(...props.data.map(line =>Math.max(...line.data.map(point => point[0]))))
  let Xmin = Math.min(...props.data.map(line =>Math.min(...line.data.map(point => point[0]))))
  let Ymax = Math.max(...props.data.map(line =>Math.max(...line.data.map(point => point[1]))))
  let Ymin = Math.min(...props.data.map(line =>Math.min(...line.data.map(point => point[1]))))

  let Xgrid = 0.5*Math.pow(10,Math.floor(Math.log10(Xmax - Xmin)))
  if ("Xgrid" in props.graph) {Xgrid = props.graph.Xgrid}
  let Ygrid = 0.5*Math.pow(10,Math.floor(Math.log10(Ymax - Ymin)))
  if ("Ygrid" in props.graph) {Ygrid = props.graph.Ygrid}

  props.data.forEach(line => {
    line.data.forEach(function (value, i) {
      if (i>0) {
        dataGraph.lines.push({
          type: line.type,
          start: {
            x: line.data[i-1][0],
            y: line.data[i-1][1]
          },
          end: {
            x: value[0],
            y: value[1]
          }
        })
      }
    });
  });

  props.lines.forEach(line => {
    line.lines.forEach((coor, i) => {
      dataGraph.lines.push({
        type: line.type,
        start: {
          x: coor[0],
          y: coor[1]
        },
        end: {
          x: coor[2],
          y: coor[3]
        }
      })
    });
  });

  props.texts.forEach(textList => {
    textList.text.forEach((text, i) => {
      dataGraph.texts.push({
        type: textList.type,
        str: text.value,
        x: text.coordinate[0],
        y: text.coordinate[1],
        angle: text.angle
      })
    });
  });

  // XAxis
  dataGraph.lines.push({
    type: 'default',
    start: {x: 0, y: Ymin}, 
    end: {x: 0, y: Ymax}
  })

  // YAxis
  dataGraph.lines.push({
    type: 'default',
    start: {x: Xmin, y: Ymax}, 
    end: {x: Xmax, y: Ymax}
  })

  // ==============================================================================
  // Xgrid + Xlabel
  let Xmax0 =  Math.ceil(Xmax/Xgrid)*Xgrid
  let Xmin0 = Math.floor(Xmin/Xgrid)*Xgrid
  // if (!("texts" in data)) {data.texts = []}
  for (let i=1; i< (((Xmax0 - Xmin0)/Xgrid)-1); i++) {
    let x = Xmin0 + i*Xgrid
    if ((x !== 0) && (x < (Xmax-Xgrid*0.5)) && (x > (Xmin + Xgrid*0.5))) {
      dataGraph.texts.push({
        type: 'XLabel',
        str: String(x),
        x: x,
        y: Ymax,
        angle: 0.0
      })
  
      dataGraph.lines.push({
        type: 'XGrid',
        start: {x: x, y: Ymin}, 
        end: {x: x, y: Ymax}
      })
    }
  }
  if (Xmin !== 0) {
    dataGraph.texts.push({
      type: 'Xmax',
      str: String(Xmin),
      x: Xmin,
      y: Ymax,
      angle: 0.0
    })
    dataGraph.lines.push({
      type: 'Xmax',
      start: {x: Xmin, y: Ymin}, 
      end: {x: Xmin, y: Ymax}
    })
  }
  
  if (Xmax !== 0) {
    dataGraph.texts.push({
      type: 'Xmax',
      str: String(Xmax),
      x: Xmax,
      y: Ymax,
      angle: 0.0
    })
    dataGraph.lines.push({
      type: 'Xmax',
      start: {x: Xmax, y: Ymin}, 
      end: {x: Xmax, y: Ymax}
    })
  }

  // ==============================================================================
  // Ylabel
  let Ymax0 = Math.ceil(Ymax/Ygrid)*Ygrid
  let Ymin0 = Math.floor(Ymin/Ygrid)*Ygrid
  for (let i=0; i< ((Ymax0 - Ymin0)/Ygrid); i++) {
    let y = Ymin0 + i*Ygrid
    if (y !== 0) {
      dataGraph.texts.push({
        type: 'YLabel',
        str: String(y),
        x: 0,
        y: y,
        angle: 0.0
      })
  
      dataGraph.lines.push({
        type: 'YGrid',
        start: {x: Xmin, y: y}, 
        end: {x: Xmax, y: y}
      })
    }
  }

  dataGraph.texts.push({
    type: 'Ymax',
    str: String(Ymin),
    x: 0,
    y: Ymin,
    angle: 0.0
  })

  dataGraph.lines.push({
    type: 'Ymax',
    start: {x: Xmin, y: Ymin}, 
    end: {x: Xmax, y: Ymin}
  })


  dataGraph.texts.push({
    type: 'Ymax',
    str: String(Ymax),
    x: 0,
    y: Ymax,
    angle: 0.0
  })

  
  // ==============================================================================
  // Chart zone
  let XchartMinPX = props.chart.padding.left.value
  let XchartMaxPX = props.chart.width.value - props.chart.padding.right.value
  let YchartMinPX = props.chart.padding.bottom.value
  let YchartMaxPX = props.chart.height.value - props.chart.padding.top.value

  // Axis zone
  let XAxisMinPX = props.chart.padding.left.value + graph.padding.left.value
  let XAxisMaxPX = props.chart.width.value - props.chart.padding.right.value - graph.padding.right.value
  let YAxisMinPX = props.chart.padding.bottom.value + graph.padding.bottom.value
  let YAxisMaxPX = props.chart.height.value - props.chart.padding.top.value - graph.padding.top.value
  
  let scaleX = (XAxisMaxPX - XAxisMinPX)/(Xmax - Xmin)
  let scaleY = (YAxisMaxPX - YAxisMinPX)/(Ymax - Ymin)
  
  // ==============================================================================
  // Translate, scale data  
  dataGraph.lines.forEach(line => {
    line.start.x = XAxisMinPX + (line.start.x - Xmin)*scaleX
    line.start.y = YAxisMinPX + (line.start.y - Ymin)*scaleY
    line.end.x = XAxisMinPX + (line.end.x - Xmin)*scaleX
    line.end.y = YAxisMinPX + (line.end.y - Ymin)*scaleY
  });

  dataGraph.texts.forEach(text => {
    text.x = XAxisMinPX + (text.x - Xmin)*scaleX
    text.y = YAxisMinPX + (text.y - Ymin)*scaleY
  });

  

  // Chart zone
  dataGraph.lines.push({
    type: 'default',
    start: {x: XchartMinPX, y: YchartMinPX}, 
    end: {x: XchartMinPX, y: YchartMaxPX}
  })
  dataGraph.lines.push({
    type: 'default',
    start: {x: XchartMinPX, y: YchartMaxPX}, 
    end: {x: XchartMaxPX, y: YchartMaxPX}
  })
  dataGraph.lines.push({
    type: 'default',
    start: {x: XchartMaxPX, y: YchartMaxPX}, 
    end: {x: XchartMaxPX, y: YchartMinPX}
  })
  dataGraph.lines.push({
    type: 'default',
    start: {x: XchartMaxPX, y: YchartMinPX}, 
    end: {x: XchartMinPX, y: YchartMinPX}
  })

  // Axis zone
  // dataGraph.lines.push({
  //   type: 'default',
  //   start: {x: XAxisMinPX, y: YAxisMinPX}, 
  //   end: {x: XAxisMinPX, y: YAxisMaxPX}
  // })
  // dataGraph.lines.push({
  //   type: 'default',
  //   start: {x: XAxisMinPX, y: YAxisMaxPX}, 
  //   end: {x: XAxisMaxPX, y: YAxisMaxPX}
  // })
  // dataGraph.lines.push({
  //   type: 'default',
  //   start: {x: XAxisMaxPX, y: YAxisMaxPX}, 
  //   end: {x: XAxisMaxPX, y: YAxisMinPX}
  // })
  // dataGraph.lines.push({
  //   type: 'default',
  //   start: {x: XAxisMaxPX, y: YAxisMinPX}, 
  //   end: {x: XAxisMinPX, y: YAxisMinPX}
  // })

  return dataGraph
}

function ChartTopDown(props) {
  
  props.graph.chart.scaleX = 1.0
  props.graph.chart.scaleY = 1.0
  props.graph.chart.offset = {
    x: 0.0,
    y: 0.0
  }

  let data = {
    lines: [],
    texts: []
  }
  // props.etats.forEach(sp => {
  //   // console.log(sp.data)
    
  // })
  // console.log(Object.keys(props.graphData));

  Object.keys(props.graph["suplots"]).forEach(key => {
    // if (Object.keys(props.graphData).includes(key)) {
    let dataGraph1 = subplot({
      graph: props.graph["suplots"][key]["graph"],
      chart:props.graph.chart,
      data: props.graph["suplots"][key]["data"].filter(k => Object.keys(props.graphData).includes(k)).map(k => props.graphData[k]),
      lines: props.graph["suplots"][key]["lines"].filter(k => Object.keys(props.graphData).includes(k)).map(k => props.graphData[k]),
      texts: props.graph["suplots"][key]["texts"].filter(k => Object.keys(props.graphData).includes(k)).map(k => props.graphData[k]),
    })
    data.lines = data.lines.concat(dataGraph1.lines)
    data.texts = data.texts.concat(dataGraph1.texts)
    // }
  });

  return (
    <div>
      <Graph graph={props.graph.chart} data={data}></Graph>
    </div>
  )
}

export default ChartTopDown;