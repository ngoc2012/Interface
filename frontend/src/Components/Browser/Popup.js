import React, {useState} from 'react';
import Browser from './Browser';
import './Browser.css';

function Popup(props) {
  const [folder, setFolder] = useState("")

  const copyto = () => {props.handle(folder, "copyto")}
  const moveto = () => {props.handle(folder, "moveto")}

  return (  
    <div className='popup'>
      <div className='popup\_inner'> 
        <div style={{backgroundColor: "#FFFFFF"}}>{folder}</div>
        <Browser {...props} showFiles={false} setCurrent={setFolder}></Browser>
        <button onClick={copyto}>Copier</button><button onClick={moveto}>Déplacer</button><button onClick={props.close}>Fermer</button>
      </div>
    </div>  
    );
}

export default Popup;