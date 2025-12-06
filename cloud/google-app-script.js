function sendEnergyBillEmail() {
  const url = 'https://nexus-4-b9919-default-rtdb.firebaseio.com/SmartMeter/SmartMeter.json';
  const response = UrlFetchApp.fetch(url);
  const data = JSON.parse(response.getContentText());

  const energy = data.energy || 0;
  const bill = data.bill || 0;
  const timestamp = data.timestamp || new Date().toString();

  const subject = `⚡ Monthly Bill ⚡ ${energy.toFixed(2)} kWh`;
  const body = `
    Hi Ishan,<br><br>

    Here is your Smart Energy Meter usage report:<br><br>

    🔌 <b>Energy Used:</b> ${energy.toFixed(2)} kWh<br>
    💰 <b>Bill Amount:</b> ₹${bill.toFixed(2)}<br>
    🕒 <b>Timestamp:</b> ${timestamp}<br><br>

    📟 Smart Meter | Firebase + Google Apps Script
  `;

  MailApp.sendEmail({
    to: "ishanjijigeorge2026@ec.ajce.in",
    subject: subject,
    htmlBody: body
  });
