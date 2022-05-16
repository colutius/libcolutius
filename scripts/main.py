from telegram.ext import Updater
from telegram import InlineKeyboardMarkup, InlineKeyboardButton
import os

os.getenv('TOKEN')
os.environ.get('TOKEN')
token = os.environ['TOKEN']

os.getenv('TO')
os.environ.get('TO')
who = os.environ['TO']

os.getenv('actor')
os.environ.get('actor')
actor = os.environ['actor']

os.getenv('message')
os.environ.get('message')
message = os.environ['message']

os.getenv('repository')
os.environ.get('repository')
repository = os.environ['repository']

os.getenv('sha')
os.environ.get('sha')
sha = os.environ['sha']

os.getenv('yiyan')
os.environ.get('yiyan')
yiyan = os.environ['yiyan']

text = f"👇新的提交👇\n\n👤提交人: [{actor}]\n\n📄提交信息: {message}\n\n📦仓库: {repository}\n\n🗓每日一言: {yiyan}"
updater = Updater(token=token)
dispatcher = updater.dispatcher

dispatcher.bot.send_message(chat_id=who, text=text, reply_markup=InlineKeyboardMarkup(
    [[InlineKeyboardButton('👀查看更改👀', url=f"https://github.com/{repository}/commit/{sha}")]]))
