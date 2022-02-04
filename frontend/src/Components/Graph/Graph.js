import React, {useEffect, useRef} from 'react';
import draw from './draw';

import './Graph.css';

function Graph(props) {

  const canvasRef = useRef(null)

  useEffect(() => {
    const canvas = canvasRef.current
    const ctx = canvas.getContext('2d');

    draw(ctx, props.graph, props.data)
  });

  const redraw = () => {
    const canvas = canvasRef.current
    const ctx = canvas.getContext('2d');
    draw(ctx, props.graph, props.data)
  }

  return (
    <div>
      <button onClick={redraw}>Redessin</button>
      <div className="container" height={props.graph.height.value}>
        <canvas ref={canvasRef} width={props.graph.width.value} height={props.graph.height.value}>Canvas</canvas>
      </div>
    </div>
  )
}

export default Graph;


// import React, { useLayoutEffect, useState } from 'react';

// function useWindowSize() {
//   const [size, setSize] = useState([0, 0]);
//   useLayoutEffect(() => {
//     function updateSize() {
//       setSize([window.innerWidth, window.innerHeight]);
//     }
//     window.addEventListener('resize', updateSize);
//     updateSize();
//     return () => window.removeEventListener('resize', updateSize);
//   }, []);
//   return size;
// }

// function ShowWindowDimensions(props) {
//   const [width, height] = useWindowSize();
//   return <span>Window size: {width} x {height}</span>;
// }