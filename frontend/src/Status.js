import React, {useState} from 'react';

export default function Status(props) { 

  const [showStatus, setShowStatus] = useState(false)

  // const pushStatus = (text) => setStatus({...status}.push(text))
  
  const toggleShowStatus = () => {
    setShowStatus(!showStatus)
  }
  // By updating the Home route with exact, now it will be rendered only if it matches the full path.
  // If not all path start with '/' will redirect to Home.
  return (
    <div className="no-print">
      <div>{props.status[props.status.length-1]}<button onClick={toggleShowStatus}>Plus</button></div>

      {showStatus && props.status.map((val, i) => {
        if (i>0) {return <div key={"status" + i}>{props.status[props.status.length-1-i]}</div>} else {return null}
      })}
    </div>
  );
}

