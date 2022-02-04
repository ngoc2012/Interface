import React, {useState} from 'react';

function getTextOutput(text, textType) {
  if (textType === "variable") {
    // let pos = newText

    // let newText = text.replaceAll("\n", "");

    // paragraph.indexOf(searchTerm);
    let newText = "";
    text.split("\n").forEach(element => {
      let pos = element.indexOf("//");
      if (element.indexOf("//") === -1) {
        newText += element;
      } else {
        newText += element.slice(0, pos);
      }
    });

    let dataTypes = ["float ", "int ", "string ", "=\"\""];
    dataTypes.forEach(e => {
      newText = newText.replaceAll(e, "");
    });
    newText = newText.replaceAll(";", ",");
    newText = newText.replaceAll(" ", "");
    
    
    // console.log(newText.split(","));
    let newText1 = "";
    const regex = /=(-?\d+(\.\d*)?((e|E)(\+|-)\d+)?)/g;
    newText.split(",").forEach(element => {
      if (element !== "") {
        let el = element.replace(regex, "");
        newText1 += "{\"" + el + "\", " + el + "},\n";
      }
    });
    navigator.clipboard.writeText(newText1);
    return newText1;
  }
  if (textType === "class") {
    let newText = text.replaceAll("\n", "");
    let newText1 = "";
    // console.log(newText.split(";"));
    newText.split(";").forEach(element => {
      if (element !== "") {
        let eleList = element.split(" ");
        let el = eleList[eleList.length-1];
        newText1 += "{\"" + el + "\", " + el + ".getJson()},\n";
      }
    });
    navigator.clipboard.writeText(newText1);
    return newText1;
  }
  return text;
}
function Parser(props) {
  // let text = "profileMetal* profile;\n\
  // flambement flambement;\n\
  // derversement derversement;";
  let text = "";

  const [textType, setTextType] = useState("class");
  const [textInput, SetTextInput] = useState(text);
  const [textOutput, SetTextOutput] = useState("smt");

  // function fetchText() {
  //   fetch(props.url.backend + "/parser", {
  //     method: 'POST',
  //     headers: {'Content-Type': 'application/json'},
  //     body: {
  //       "text": textInput,
  //     }
  //   })
  //   .then(response => response.json())
  //   .then(data => SetTextInput({'type': 'setDataset', 'dataset': data}))
  //   .catch(error => {console.error('Error:', error);}); 
  // }
  
  // useEffect(fetchFile, [params, props])
  
  function onClickButton() {
    // fetchText();
    SetTextOutput(getTextOutput(textInput, textType));
    // console.log("smt")
    
  }

  function onChangeInput(e) {
    SetTextInput(e.target.value)
  }

  function onChangeType(e){
    setTextType(e.target.value)
    SetTextOutput(e.target.value)
  }

  return <div>
    <select onChange={onChangeType} value={textType}>
      <option value="variable">variable</option>
      <option value="class">class</option>
    </select><br/>
    <textarea onChange={onChangeInput} value={textInput} rows="10" cols="100"></textarea><br/>
    <button onClick={onClickButton}>Get</button>
    <div>{textOutput}</div>
    
  </div>
  
  
  
}

export default Parser;