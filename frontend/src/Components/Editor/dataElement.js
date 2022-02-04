import React, {useState, useContext} from 'react';
import {DatasetDispatch} from './Editor';

export function InputData(props) {
  
  // console.log(props.value)
  const [editable, setEditable] = useState(false)
  const [text, setText] = useState(props.value)
  const datasetDispatch = useContext(DatasetDispatch);

  function updateInput() {
    setEditable(false)
    datasetDispatch({
      'type': 'updateInput',
      'attr': props.attr,
      'value': parseFloat(text)});
  }

  const inputChange = (e) => {setText(e.target.value)}

  return <span onClick={() => {setEditable(true)}}>
    {editable && (
      <form
        onSubmit={e => {
          e.preventDefault();
          updateInput()
      }}>
      <input 
        type="text"
        value={text} 
        onChange={inputChange}
        // size={size}
        onBlur={updateInput}
        // onFocus={onFocusHandle}
      ></input>
    </form>
    )}
    
    {!editable && (<span>{text}</span>)}
    </span>
}

